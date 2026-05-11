#pragma once
#include "AFractals.hpp"

class SphereMarching : public AFractals {

public:
  SphereMarching(const libconfig::Setting &s);
  float evaluateSDF(const Vec3 &p) const override;
  AABB getObjectAABB() const override;
  Vec3 getCentroid() const override;

private:
  float _radius;
};
