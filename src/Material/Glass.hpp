#pragma once

#include "IMaterials.hpp"

class Glass : public Material {
public:
  Glass(const libconfig::Setting &s);
  Glass();
};