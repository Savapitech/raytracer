#include "Ray.hpp"
#include "logger.hpp"

namespace RayTracer 
{
    RayTracer::RayTracer(const CmdConfig::config_t &config) :
        scene(config.scene),
        render(scene, config)
    {
        Log::Logger::info("Start Ray Tracer:");
    }

    void RayTracer::run(void) {
        Log::Logger::info("Start Running...");
        this->render.runRender();   
    }
}

