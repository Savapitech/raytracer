#include "Ray.hpp"
#include "logger.hpp"

namespace RayTracer 
{
    RayTracer::RayTracer(const CmdConfig::config_t &config) :
        _scene(config.scene),
        _render(_scene, config)
    {
        Log::Logger::info("Start Ray Tracer:");
    }

    void RayTracer::run(void) {
        Log::Logger::info("Start Running...");
        _render.runRender();
    }
}

