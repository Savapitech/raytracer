#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Cluster {

class ClusterSlave {
public:
  ClusterSlave(const std::string &host, int port);
  void run();

private:
  bool connect();
  void runSession();
  void sendTilePixels(const std::vector<uint32_t> &tiles,
                      const std::vector<uint8_t> &fb, int width, int tilesX);

  std::string _host;
  int _port;
  int _fd = -1;
};

} // namespace Cluster
