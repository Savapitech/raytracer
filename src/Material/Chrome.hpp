#pragma once

#include "Mirror.hpp"

class Chrome : public Mirror {
public:
  Chrome(const libconfig::Setting &s);
  Chrome();
};