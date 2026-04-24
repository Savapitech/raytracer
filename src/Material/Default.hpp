#pragma once

#include "IMaterials.hpp"

class Default final : public AMaterial
{
    public:
        Default(const libconfig::Setting& s);
        Default();
};
