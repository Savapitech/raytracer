#pragma once

#include <iostream>
#include "CmdConfig.hpp"

class RayTracer
{
    public:
        RayTracer(const CmdConfig::config_t &config);
    private:
        const CmdConfig::config_t &config;
};