#pragma once

namespace CmdConfig {
    struct config_t
    {
        std::string LogLvl;

        std::string scene;

        bool pathtracing = false;

        int sample = 1;

        std::string output;

        bool gui = false;
    };
}
