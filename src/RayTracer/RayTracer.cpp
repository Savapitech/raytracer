#include "RayTracer.hpp"
#include "logger.hpp"

RayTracer::RayTracer(const CmdConfig::config_t &config) :
    config(config) 
{
    Log::Logger::info("Start Ray Tracer");
}