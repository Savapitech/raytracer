#include "RayTracer.hpp"
#include "logger.hpp"

namespace RayTracer {

RayTracer::RayTracer(const CmdConfig::config_t &config) :
    config(config),
    scene(config.scene),
    render(scene.getObject())
    {
        Log::Logger::info("Start Ray Tracer");
    }
}