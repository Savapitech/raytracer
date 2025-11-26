#pragma once

#include <iostream>
#include "CmdConfig.hpp"

namespace RayTracer {

    
    class RayTracer
    {
        public:
            RayTracer(const CmdConfig::config_t &config);
            void run();
    
        private:
            const CmdConfig::config_t &config;
    };
}