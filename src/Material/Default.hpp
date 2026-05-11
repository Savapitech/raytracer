#pragma once

#include "IMaterials.hpp"

class Default final : public Material {
public:
  Default(const libconfig::Setting &s);
  Default();
};
