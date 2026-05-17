/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** raytracer
*/
#include "ClusterServer.hpp"
#include "ClusterSlave.hpp"
#include "Object.hpp"
#include "ParserCmd.hpp"
#include "Ray.hpp"
#include "logger.hpp"


namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

using namespace object;
using namespace render;
using namespace graphical;
using namespace space;
using namespace shape;
using namespace material;
using namespace light;


static int parsePort(const std::string &s) { return std::stoi(s); }

static std::pair<std::string, int> parseHostPort(const std::string &s) {
  auto pos = s.rfind(':');
  if (pos == std::string::npos)
    throw std::invalid_argument("--slave expects host:port, got: " + s);
  return {s.substr(0, pos), std::stoi(s.substr(pos + 1))};
}

int main(int ac, char **av) {
  Log::Logger::SetLogLvl(Log::Logger::DEBUG);
  Log::Logger::info("start...");

  ParserCmd::Parser parser(ac, av);

  try {
    parser.initParser();
    parser.buildConfig();

    const auto &cfg = parser.getConfig();

    // --- Cluster server mode ---
    if (!cfg.clusterServer.empty()) {
      auto config = cfg;
      config.clusterPort = parsePort(cfg.clusterServer);
      scene::Scene scene(config.scene);
      Cluster::ClusterServer server(config, scene);
      server.run();
      return 0;
    }

    // --- Cluster slave mode ---
    if (!cfg.clusterSlave.empty()) {
      auto [host, port] = parseHostPort(cfg.clusterSlave);
      Cluster::ClusterSlave slave(host, port);
      slave.run();
      return 0;
    }

    // --- Normal local render ---
    if (cfg.gui == true)
      gui();

    RayTracer::RayTracer raytracer(cfg);
    raytracer.run();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}