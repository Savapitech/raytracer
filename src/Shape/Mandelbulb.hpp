#pragma once
#include "AFractals.hpp"
#include <cmath>

class Mandelbulb : public AFractals {
public:
  Mandelbulb(const libconfig::Setting &s);
  float evaluateSDF(const Vec3 &p) const override;
  AABB getObjectAABB() const override;
  Vec3 getCentroid() const override;

private:
  float _power;
  int _iterations;
  bool _juliaMode;
  Vec3 _juliaConstant;
};