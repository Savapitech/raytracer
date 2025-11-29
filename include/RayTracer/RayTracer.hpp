#pragma once

#include <iostream>
#include "CmdConfig.hpp"
#include "Scene.hpp"
#include "render.hpp"

namespace RayTracer {

    
    class RayTracer
    {
        public:
            RayTracer(const CmdConfig::config_t &config);
            void run();
    
        private:
            const CmdConfig::config_t &config;
            const scene::Scene scene;
            Render render;
    };
}