#pragma once

#include <iostream>
#include <logger.hpp>

namespace CmdConfig {
    struct config_t
    {
        int LogLvl = Log::Logger::NONE;

        std::string scene;

        bool pathtracing;
    };
}