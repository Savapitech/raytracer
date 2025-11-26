#include "RayTracer.hpp"
#include "logger.hpp"

namespace RayTracer {

RayTracer::RayTracer(const CmdConfig::config_t &config) :
    config(config) 
    {
        Log::Logger::info("Start Ray Tracer");
    }
}