#include <iostream>

#include "RayTracer.hpp"
#include "logger.hpp"

namespace RayTracer {

void RayTracer::run(void) {
        Log::Logger::info("Start Running...");
        if (this->config.realTimeCPU == false)
            this->render.RunRender();
        else
            this->render.RunRenderRT();   
    }
}