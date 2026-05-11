#pragma once
#include "IShape.hpp"

class Plane : public AShape {
public:
  Plane(const libconfig::Setting &s);
  bool intersect(Ray &ray, Hit &hit) const noexcept override;
  AABB getObjectAABB() const noexcept override;
  Vec3 getCentroid() const noexcept override;
  Vec2 getUv(Vec3 &hitPos) const noexcept override;

private:
  Vec3 _normale;
};
