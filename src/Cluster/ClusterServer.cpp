#include "ClusterServer.hpp"
#include "ClusterProtocol.hpp"
#include "logger.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>
#include <libconfig.h++>
#include <set>
#include <sstream>
#include <stdexcept>
#include <thread>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>


namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

namespace Cluster {
    using namespace render;
    using namespace object;
    using namespace space;
    using namespace shape;
    using namespace material;
    using namespace light;
    using namespace graphical;


static std::string dirOf(const std::string &p) {
  auto pos = p.rfind('/');
  return pos == std::string::npos ? "." : p.substr(0, pos);
}

static std::string joinPath(const std::string &dir, const std::string &file) {
  if (file.empty() || file[0] == '/')
    return file;
  if (dir == "." || dir.empty())
    return file;
  return dir + "/" + file;
}

// Extract just the filename from a path (handles both / and \ separators)
static std::string filenameOf(const std::string &p) {
  size_t pos = p.find_last_of("/\\");
  return pos == std::string::npos ? p : p.substr(pos + 1);
}

static std::string extOf(const std::string &p) {
  auto dot = p.rfind('.');
  if (dot == std::string::npos)
    return {};
  auto slash = p.rfind('/');
  if (slash != std::string::npos && slash > dot)
    return {};
  std::string ext = p.substr(dot);
  for (auto &c : ext)
    c = static_cast<char>(std::tolower(c));
  return ext;
}

void ClusterServer::collectFile(const std::string &relPath,
                                const std::string &sceneDir,
                                std::vector<Asset> &out,
                                std::set<std::string> &visited) const {
  std::string absPath = joinPath(sceneDir, relPath);
  if (!visited.insert(absPath).second)
    return;

  std::ifstream f(absPath, std::ios::binary);
  if (!f)
    return; // file not found — skip

  std::vector<uint8_t> content((std::istreambuf_iterator<char>(f)), {});
  out.push_back({relPath, content});

  std::string ext = extOf(relPath);
  std::string fileDir = joinPath(sceneDir, dirOf(relPath));

  if (ext == ".obj") {
    std::istringstream ss(std::string(content.begin(), content.end()));
    std::string line;
    while (std::getline(ss, line)) {
      if (line.size() > 7 && line.substr(0, 6) == "mtllib") {
        std::string mtl = line.substr(7);
        while (!mtl.empty() && (mtl.back() == '\r' || mtl.back() == ' '))
          mtl.pop_back();
        collectFile(joinPath(dirOf(relPath), mtl), sceneDir, out, visited);
      }
    }
  } else if (ext == ".mtl") {
    static const std::vector<std::string> mapKeys = {
        "map_Kd",   "map_Ka", "map_Ks", "map_Ns", "map_d",
        "map_bump", "bump",   "disp",   "norm"};
    std::istringstream ss(std::string(content.begin(), content.end()));
    std::string line;
    while (std::getline(ss, line)) {
      size_t start = line.find_first_not_of(" \t");
      if (start == std::string::npos)
        continue;
      std::string trimmed = line.substr(start);
      while (!trimmed.empty() &&
             (trimmed.back() == '\r' || trimmed.back() == ' '))
        trimmed.pop_back();

      for (const auto &key : mapKeys) {
        if (trimmed.size() > key.size() + 1 &&
            trimmed.substr(0, key.size()) == key) {
          std::istringstream ls(trimmed);
          std::string tok, last;
          while (ls >> tok)
            if (tok[0] != '-')
              last = tok;
          if (last.empty())
            continue;

          std::string fname = filenameOf(last);
          std::string mtlDir = dirOf(relPath);
          for (const char *sub : {"", "textures/", "Textures/"}) {
            std::string candidate = joinPath(mtlDir, sub + fname);
            std::string abs = joinPath(sceneDir, candidate);
            if (std::ifstream(abs).good()) {
              collectFile(candidate, sceneDir, out, visited);
              break;
            }
          }
        }
      }
    }
  }
}

static void gatherStrings(const libconfig::Setting &s,
                          std::vector<std::string> &out) {
  if (s.getType() == libconfig::Setting::TypeString) {
    out.emplace_back(static_cast<const char *>(s));
    return;
  }
  for (int i = 0; i < s.getLength(); i++)
    gatherStrings(s[i], out);
}

std::vector<Asset> ClusterServer::collectAssets() const {
  static const std::set<std::string> knownExts = {
      ".obj", ".mtl", ".hdr", ".png", ".jpg", ".jpeg", ".bmp", ".tga", ".ppm"};

  std::vector<Asset> assets;
  std::set<std::string> visited;
  std::string sceneDir = dirOf(_config.scene);

  libconfig::Config cfg;
  cfg.readString(_sceneContent);
  std::vector<std::string> strings;
  gatherStrings(cfg.getRoot(), strings);

  for (const auto &val : strings) {
    if (knownExts.count(extOf(val)))
      collectFile(val, sceneDir, assets, visited);
  }

  Log::Logger::info("Collected " + std::to_string(assets.size()) +
                    " asset(s) to send to slaves");
  return assets;
}

ClusterServer::ClusterServer(const CmdConfig::config_t &config,
                             scene::Scene &scene)
    : _config(config), _scene(scene) {
  const auto &cam = scene.getCamera();
  _width = cam.width;
  _height = cam.height;
  _tilesX = (_width + TILE_SIZE - 1) / TILE_SIZE;
  int tilesY = (_height + TILE_SIZE - 1) / TILE_SIZE;
  _totalTiles = _tilesX * tilesY;

  _framebuffer.resize(_width * _height * 4, 0);
  for (int i = 3; i < _width * _height * 4; i += 4)
    _framebuffer[i] = 255;

  std::ifstream f(config.scene);
  if (!f)
    throw std::runtime_error("Cannot open scene: " + config.scene);
  std::ostringstream ss;
  ss << f.rdbuf();
  _sceneContent = ss.str();

  _assets = collectAssets();

  buildTileQueue();

  _listenFd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (_listenFd < 0)
    throw std::runtime_error("socket() failed");
  int opt = 1;
  ::setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(static_cast<uint16_t>(config.clusterPort));
  if (::bind(_listenFd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0)
    throw std::runtime_error("bind() failed on port " +
                             std::to_string(config.clusterPort));
  if (::listen(_listenFd, 16) < 0)
    throw std::runtime_error("listen() failed");

  Log::Logger::info("Cluster server listening on port " +
                    std::to_string(config.clusterPort) + " — " +
                    std::to_string(_totalTiles) + " tiles total");
}

ClusterServer::~ClusterServer() {
  if (_listenFd >= 0)
    ::close(_listenFd);
}

void ClusterServer::buildTileQueue() {
  for (int i = 0; i < _totalTiles; i++)
    _tileQueue.push(static_cast<uint32_t>(i));
}

void ClusterServer::resetRender() {
  {
    std::lock_guard<std::mutex> lock(_tileMutex);
    while (!_tileQueue.empty())
      _tileQueue.pop();
    buildTileQueue();
  }
  _completedTiles = 0;
  _renderVersion.fetch_add(1, std::memory_order_relaxed);
}

bool ClusterServer::handleMovement() {
  const auto &cam = _scene.getCamera();
  bool move = false;
  float rotSpeed = 0.05f;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
    _scene.updateCamera({cam.pos.x - 0.2f, cam.pos.y, cam.pos.z});
    move = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
    _scene.updateCamera({cam.pos.x + 0.2f, cam.pos.y, cam.pos.z});
    move = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
    _scene.updateCamera({cam.pos.x, cam.pos.y, cam.pos.z - 0.2f});
    move = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
    _scene.updateCamera({cam.pos.x, cam.pos.y, cam.pos.z + 0.2f});
    move = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
    _scene.updateCamera({cam.pos.x, cam.pos.y + 0.2f, cam.pos.z});
    move = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
    _scene.updateCamera({cam.pos.x, cam.pos.y - 0.2f, cam.pos.z});
    move = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad6)) {
    Vec3 d;
    d.x = cam.dir.x * cos(-rotSpeed) - cam.dir.z * sin(-rotSpeed);
    d.y = cam.dir.y;
    d.z = cam.dir.x * sin(-rotSpeed) + cam.dir.z * cos(-rotSpeed);
    _scene.updateCameraDir(d);
    move = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad4)) {
    Vec3 d;
    d.x = cam.dir.x * cos(rotSpeed) - cam.dir.z * sin(rotSpeed);
    d.y = cam.dir.y;
    d.z = cam.dir.x * sin(rotSpeed) + cam.dir.z * cos(rotSpeed);
    _scene.updateCameraDir(d);
    move = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad8)) {
    _scene.updateCameraDir(cam.dir * cos(rotSpeed) + cam.up * sin(rotSpeed));
    move = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad5)) {
    _scene.updateCameraDir(cam.dir * cos(-rotSpeed) + cam.up * sin(-rotSpeed));
    move = true;
  }
  return move;
}

std::vector<uint8_t> ClusterServer::buildRerenderPayload() const {
  const auto &cam = _scene.getCamera();
  std::vector<uint8_t> p(24);
  std::memcpy(p.data() + 0, &cam.pos.x, 4);
  std::memcpy(p.data() + 4, &cam.pos.y, 4);
  std::memcpy(p.data() + 8, &cam.pos.z, 4);
  std::memcpy(p.data() + 12, &cam.dir.x, 4);
  std::memcpy(p.data() + 16, &cam.dir.y, 4);
  std::memcpy(p.data() + 20, &cam.dir.z, 4);
  return p;
}

std::vector<uint32_t> ClusterServer::assignTiles(int count) {
  std::lock_guard<std::mutex> lock(_tileMutex);
  std::vector<uint32_t> tiles;
  tiles.reserve(static_cast<size_t>(count));
  while (!_tileQueue.empty() && static_cast<int>(tiles.size()) < count) {
    tiles.push_back(_tileQueue.front());
    _tileQueue.pop();
  }
  return tiles;
}

void ClusterServer::compositeUpdate(const std::vector<uint8_t> &payload) {
  if (payload.size() < 4)
    return;
  const uint8_t *p = payload.data();
  uint32_t numTiles = readU32(p);
  p += 4;
  const int tileBytes = TILE_SIZE * TILE_SIZE * 4;
  const uint8_t *end = payload.data() + payload.size();

  std::lock_guard<std::mutex> lock(_fbMutex);
  for (uint32_t t = 0; t < numTiles; t++) {
    if (p + 4 + tileBytes > end)
      break;
    uint32_t idx = readU32(p);
    p += 4;
    const uint8_t *src = p;
    p += tileBytes;

    int tx = static_cast<int>(idx % static_cast<uint32_t>(_tilesX)) * TILE_SIZE;
    int ty = static_cast<int>(idx / static_cast<uint32_t>(_tilesX)) * TILE_SIZE;
    int endX = std::min(tx + TILE_SIZE, _width);
    int endY = std::min(ty + TILE_SIZE, _height);

    for (int y = ty; y < endY; y++) {
      for (int x = tx; x < endX; x++) {
        int srcOff = ((y - ty) * TILE_SIZE + (x - tx)) * 4;
        int dstOff = (y * _width + x) * 4;
        _framebuffer[dstOff + 0] = src[srcOff + 0];
        _framebuffer[dstOff + 1] = src[srcOff + 1];
        _framebuffer[dstOff + 2] = src[srcOff + 2];
        _framebuffer[dstOff + 3] = src[srcOff + 3];
      }
    }
    _completedTiles.fetch_add(1, std::memory_order_relaxed);
  }
}

static bool sendTileAssign(int fd, const std::vector<uint32_t> &tiles) {
  std::vector<uint8_t> assign;
  writeU32(assign, static_cast<uint32_t>(tiles.size()));
  for (auto idx : tiles)
    writeU32(assign, idx);
  return sendMsg(fd, MSG_TILE_ASSIGN, assign);
}

void ClusterServer::handleSlave(int clientFd) {
  uint8_t type;
  std::vector<uint8_t> payload;

  if (!recvMsg(clientFd, type, payload) || type != MSG_HELLO) {
    ::close(clientFd);
    return;
  }
  Log::Logger::info("New slave connected");

  {
    std::vector<uint8_t> job;
    writeU32(job, static_cast<uint32_t>(_width));
    writeU32(job, static_cast<uint32_t>(_height));
    writeU32(job, static_cast<uint32_t>(_config.sample));
    job.push_back(_config.pathtracing ? 1 : 0);
    writeU32(job, static_cast<uint32_t>(_sceneContent.size()));
    job.insert(job.end(), _sceneContent.begin(), _sceneContent.end());
    // Assets
    writeU32(job, static_cast<uint32_t>(_assets.size()));
    for (const auto &a : _assets) {
      writeU32(job, static_cast<uint32_t>(a.relPath.size()));
      job.insert(job.end(), a.relPath.begin(), a.relPath.end());
      writeU32(job, static_cast<uint32_t>(a.content.size()));
      job.insert(job.end(), a.content.begin(), a.content.end());
    }
    if (!sendMsg(clientFd, MSG_JOB, job)) {
      ::close(clientFd);
      return;
    }
  }

  {
    auto tiles = assignTiles(TILES_PER_BATCH);
    if (!tiles.empty()) {
      if (!sendTileAssign(clientFd, tiles)) {
        ::close(clientFd);
        return;
      }
    } else {
      sendMsg(clientFd, MSG_NO_MORE, {});
    }
  }

  uint32_t myVersion = _renderVersion.load(std::memory_order_relaxed);

  while (_running) {
    uint32_t curVersion = _renderVersion.load(std::memory_order_relaxed);
    if (curVersion != myVersion) {
      myVersion = curVersion;
      if (!sendMsg(clientFd, MSG_RERENDER, buildRerenderPayload()))
        break;
      auto tiles = assignTiles(TILES_PER_BATCH);
      if (tiles.empty()) {
        sendMsg(clientFd, MSG_NO_MORE, {});
      } else if (!sendTileAssign(clientFd, tiles))
        break;
      continue;
    }

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(clientFd, &rfds);
    timeval tv{0, 50000};
    int ret = ::select(clientFd + 1, &rfds, nullptr, nullptr, &tv);
    if (ret < 0)
      break;
    if (ret == 0)
      continue;

    if (!recvMsg(clientFd, type, payload))
      break;

    if (type == MSG_FRAME_UPDATE) {
      compositeUpdate(payload);
    } else if (type == MSG_REQUEST_MORE) {
      auto more = assignTiles(TILES_PER_BATCH);
      if (more.empty())
        sendMsg(clientFd, MSG_NO_MORE, {});
      else
        sendTileAssign(clientFd, more);
    } else if (type == MSG_ALL_DONE) {
      Log::Logger::info("Slave finished, waiting for next render");
    }
  }
  ::close(clientFd);
}

void ClusterServer::acceptLoop() {
  while (_running) {
    sockaddr_in clientAddr{};
    socklen_t addrLen = sizeof(clientAddr);
    int clientFd = ::accept(
        _listenFd, reinterpret_cast<sockaddr *>(&clientAddr), &addrLen);
    if (clientFd < 0) {
      if (!_running)
        break;
      continue;
    }
    std::thread([this, clientFd] { handleSlave(clientFd); }).detach();
  }
}

void ClusterServer::run() {
  std::thread acceptThread([this] { acceptLoop(); });

  sf::Texture texture(sf::Vector2u(static_cast<unsigned>(_width),
                                   static_cast<unsigned>(_height)));
  sf::Sprite sprite(texture);
  sf::RenderWindow window(
      sf::VideoMode(sf::Vector2u(static_cast<unsigned>(_width),
                                 static_cast<unsigned>(_height))),
      "RayTracer - Cluster Server");

  auto lastDisplay = std::chrono::steady_clock::now();
  bool renderDone = false;

  Log::Logger::info("Waiting for slaves...");

  while (window.isOpen()) {
    while (auto event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    if (handleMovement()) {
      resetRender();
      renderDone = false;
    }

    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDisplay)
            .count();
    if (elapsed >= DISPLAY_INTERVAL_MS) {
      {
        std::lock_guard<std::mutex> lock(_fbMutex);
        texture.update(_framebuffer.data());
      }
      sprite.setTexture(texture);
      lastDisplay = now;
    }

    window.clear();
    window.draw(sprite);
    window.display();

    if (!renderDone &&
        _completedTiles.load(std::memory_order_relaxed) >= _totalTiles) {
      Log::Logger::info("Render complete");
      renderDone = true;
    }
  }

  _running = false;
  ::shutdown(_listenFd, SHUT_RDWR);
  acceptThread.join();
}

} // namespace Cluster
