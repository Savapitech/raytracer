#pragma once

#include "IMaterials.hpp"

class Glass : public AMaterial  
{
    public:
        Glass(const libconfig::Setting& s);
        Glass();
};