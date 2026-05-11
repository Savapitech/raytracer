#pragma once

#include "IMaterials.hpp"

class Mirror : public Material {
public:
  Mirror(const libconfig::Setting &s);
  Mirror();

  float reflectlvl;
};