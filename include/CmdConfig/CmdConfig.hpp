#pragma once

#include "logger.hpp"

#include <iostream>

namespace CmdConfig {
    struct config_t
    {
        std::string LogLvl;

        std::string scene;

        bool pathtracing;
    };
}
