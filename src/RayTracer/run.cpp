#include <iostream>

#include "RayTracer.hpp"
#include "logger.hpp"

namespace RayTracer {

void RayTracer::run(void) {
        Log::Logger::info("Start Running...");
        this->render.RunRender();   
    }
}