#pragma once

#include "IShape.hpp"

class CylinderInf : public AShape {
public:
  CylinderInf(const libconfig::Setting &s);
  bool intersect(Ray &ray, Hit &hit) const noexcept override;
  AABB getObjectAABB() const noexcept override;
  Vec3 getCentroid() const noexcept override;
  Vec2 getUv(Vec3 &hitPos) const noexcept override;

private:
  float _heigth;
  float _radius;
  Vec3 _dir;
};
