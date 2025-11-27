#pragma once

#include "logger.hpp"

#include <iostream>

namespace CmdConfig {
    struct config_t
    {
        int LogLvl = Log::Logger::NONE;

        std::string scene;

        bool pathtracing;
    };
}
