#include "ClusterSlave.hpp"
#include "ClusterProtocol.hpp"
#include "CmdConfig.hpp"
#include "Render.hpp"
#include "Scene.hpp"
#include "logger.hpp"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <future>
#include <stdexcept>
#include <thread>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace Cluster {

ClusterSlave::ClusterSlave(const std::string &host, int port)
    : _host(host), _port(port) {}

bool ClusterSlave::connect() {
  if (_fd >= 0) {
    ::close(_fd);
    _fd = -1;
  }
  _fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (_fd < 0)
    return false;
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(static_cast<uint16_t>(_port));
  if (::inet_pton(AF_INET, _host.c_str(), &addr.sin_addr) <= 0) {
    ::close(_fd);
    _fd = -1;
    return false;
  }
  if (::connect(_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
    ::close(_fd);
    _fd = -1;
    return false;
  }
  return true;
}

void ClusterSlave::sendTilePixels(const std::vector<uint32_t> &tiles,
                                  const std::vector<uint8_t> &fb, int width,
                                  int tilesX) {
  const int tileBytes = TILE_SIZE * TILE_SIZE * 4;
  std::vector<uint8_t> msg;
  msg.reserve(4 + tiles.size() * (4 + static_cast<size_t>(tileBytes)));
  writeU32(msg, static_cast<uint32_t>(tiles.size()));

  for (uint32_t idx : tiles) {
    writeU32(msg, idx);
    int tx = static_cast<int>(idx % static_cast<uint32_t>(tilesX)) * TILE_SIZE;
    int ty = static_cast<int>(idx / static_cast<uint32_t>(tilesX)) * TILE_SIZE;
    int endX = std::min(tx + TILE_SIZE, width);
    int endY = std::min(ty + TILE_SIZE, (int)(fb.size() / (width * 4)));

    size_t base = msg.size();
    msg.resize(base + static_cast<size_t>(tileBytes), 0);
    for (int y = ty; y < endY; y++) {
      for (int x = tx; x < endX; x++) {
        int src = (y * width + x) * 4;
        int dst =
            static_cast<int>(base) + ((y - ty) * TILE_SIZE + (x - tx)) * 4;
        msg[dst + 0] = fb[src + 0];
        msg[dst + 1] = fb[src + 1];
        msg[dst + 2] = fb[src + 2];
        msg[dst + 3] = fb[src + 3];
      }
    }
  }
  sendMsg(_fd, MSG_FRAME_UPDATE, msg);
}

void ClusterSlave::runSession() {
  sendMsg(_fd, MSG_HELLO, {});

  uint8_t type;
  std::vector<uint8_t> payload;
  if (!recvMsg(_fd, type, payload) || type != MSG_JOB)
    throw std::runtime_error("Expected MSG_JOB");

  const uint8_t *p = payload.data();
  int width = static_cast<int>(readU32(p));
  p += 4;
  int height = static_cast<int>(readU32(p));
  p += 4;
  int samples = static_cast<int>(readU32(p));
  p += 4;
  bool pathtracing = (*p++ != 0);
  uint32_t sceneLen = readU32(p);
  p += 4;
  std::string sceneContent(reinterpret_cast<const char *>(p), sceneLen);
  p += sceneLen;
  uint32_t numAssets = readU32(p);
  p += 4;

  int tilesX = (width + TILE_SIZE - 1) / TILE_SIZE;
  Log::Logger::info("Job: " + std::to_string(width) + "x" +
                    std::to_string(height) +
                    " samples=" + std::to_string(samples) +
                    " assets=" + std::to_string(numAssets));

  char tmpBuf[] = "/tmp/cluster_XXXXXX";
  if (!mkdtemp(tmpBuf))
    throw std::runtime_error("mkdtemp failed");
  std::string workDir(tmpBuf);

  std::string scenePath = workDir + "/scene.cfg";
  {
    std::ofstream f(scenePath);
    f << sceneContent;
  }

  for (uint32_t i = 0; i < numAssets; i++) {
    uint32_t pathLen = readU32(p);
    p += 4;
    std::string relPath(reinterpret_cast<const char *>(p), pathLen);
    p += pathLen;
    uint32_t fileLen = readU32(p);
    p += 4;

    std::filesystem::path dest = std::filesystem::path(workDir) / relPath;
    std::filesystem::create_directories(dest.parent_path());
    std::ofstream f(dest, std::ios::binary);
    f.write(reinterpret_cast<const char *>(p), fileLen);
    p += fileLen;
  }

  if (::chdir(workDir.c_str()) != 0)
    throw std::runtime_error("chdir to workDir failed");

  CmdConfig::config_t cfg;
  cfg.scene = "scene.cfg";
  cfg.sample = samples;
  cfg.pathtracing = pathtracing;
  cfg.headless = true;

  scene::Scene scene("scene.cfg");
  Render render(scene, cfg);
  render.buildBVH();

  const auto &fb = render.getFrameBuffer();
  int numThreads = std::max(1, (int)std::thread::hardware_concurrency() - 1);
  Log::Logger::info("Slave ready — " + std::to_string(numThreads) +
                    " render threads");

  while (true) {
    if (!recvMsg(_fd, type, payload))
      throw std::runtime_error("Connection lost");

    if (type == MSG_RERENDER) {
      if (payload.size() >= 24) {
        float pos[3], dir[3];
        std::memcpy(pos, payload.data(), 12);
        std::memcpy(dir, payload.data() + 12, 12);
        scene.updateCamera({pos[0], pos[1], pos[2]});
        scene.updateCameraDir({dir[0], dir[1], dir[2]});
      }
      continue;
    }

    if (type == MSG_NO_MORE)
      continue;

    if (type != MSG_TILE_ASSIGN)
      continue;

    uint32_t count = readU32(payload.data());
    std::vector<uint32_t> tiles;
    tiles.reserve(count);
    for (uint32_t i = 0; i < count; i++)
      tiles.push_back(readU32(payload.data() + 4 + i * 4));

    Log::Logger::info("Rendering " + std::to_string(tiles.size()) + " tiles");

    auto fut = std::async(std::launch::async, [&] {
      render.renderSubset(tiles, tilesX, numThreads);
    });
    while (fut.wait_for(std::chrono::milliseconds(SLAVE_UPDATE_INTERVAL_MS)) !=
           std::future_status::ready) {
      sendTilePixels(tiles, fb, width, tilesX);
    }
    sendTilePixels(tiles, fb, width, tilesX);

    sendMsg(_fd, MSG_REQUEST_MORE, {});
  }

  std::filesystem::remove_all(workDir);
}

void ClusterSlave::run() {
  char originalCwd[4096];
  if (!getcwd(originalCwd, sizeof(originalCwd)))
    throw std::runtime_error("getcwd failed");

  while (true) {
    Log::Logger::info("Slave: connecting to " + _host + ":" +
                      std::to_string(_port) + "...");
    if (!connect()) {
      Log::Logger::info("Slave: connection failed, retrying in 3s");
      std::this_thread::sleep_for(std::chrono::seconds(3));
      continue;
    }
    Log::Logger::info("Slave: connected");
    ::chdir(originalCwd);
    try {
      runSession();
    } catch (const std::exception &e) {
      Log::Logger::info(std::string("Slave: session ended (") + e.what() +
                        "), reconnecting in 3s");
    }
    if (_fd >= 0) {
      ::close(_fd);
      _fd = -1;
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
  }
}

} // namespace Cluster
