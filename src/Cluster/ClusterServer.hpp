#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <atomic>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "CmdConfig.hpp"
#include "Scene.hpp"

namespace Cluster {

struct Asset {
  std::string relPath;
  std::vector<uint8_t> content;
};

class ClusterServer {
public:
  ClusterServer(const CmdConfig::config_t &config, scene::Scene &scene);
  ~ClusterServer();
  void run();

private:
  void buildTileQueue();
  void acceptLoop();
  void handleSlave(int clientFd);
  std::vector<uint32_t> assignTiles(int count);
  void compositeUpdate(const std::vector<uint8_t> &payload);
  void resetRender();
  bool handleMovement();
  std::vector<uint8_t> buildRerenderPayload() const;

  std::vector<Asset> collectAssets() const;
  void collectFile(const std::string &relPath, const std::string &sceneDir,
                   std::vector<Asset> &out,
                   std::set<std::string> &visited) const;

  CmdConfig::config_t _config;
  scene::Scene &_scene;
  int _listenFd = -1;

  int _width;
  int _height;
  int _tilesX;
  int _totalTiles;
  std::string _sceneContent;
  std::vector<Asset> _assets;

  std::vector<uint8_t> _framebuffer;
  std::mutex _fbMutex;

  std::queue<uint32_t> _tileQueue;
  std::mutex _tileMutex;

  std::atomic<int> _completedTiles{0};
  std::atomic<bool> _running{true};
  std::atomic<uint32_t> _renderVersion{0};
};

} // namespace Cluster
