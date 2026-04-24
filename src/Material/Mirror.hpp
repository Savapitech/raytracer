#pragma once

#include "IMaterials.hpp"

class Mirror : public AMaterial  
{
    public:
        Mirror(const libconfig::Setting& s);
        Mirror() = default;

        float reflectlvl;
};