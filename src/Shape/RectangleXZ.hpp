#pragma once
#include "IShape.hpp"

class RectangleXZ : public AShape {
public:
  RectangleXZ(const libconfig::Setting &s);
  bool intersect(Ray &ray, Hit &hit) const noexcept override;
  AABB getObjectAABB() const noexcept override;
  Vec3 getCentroid() const noexcept override;
  Vec2 getUv(Vec3 &hitPos) const noexcept override;

  Vec2 x;
  float y;
  Vec2 z;
};
