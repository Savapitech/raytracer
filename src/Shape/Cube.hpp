#pragma once
#include "IShape.hpp"

class Cube final : public AShape {
public:
  bool intersect(Ray &ray, Hit &hit) const noexcept override;
  AABB getObjectAABB() const noexcept override;
  Vec3 getCentroid() const noexcept override;
  Vec2 getUv(Vec3 &hitPos) const noexcept override;
  Cube(const libconfig::Setting &s);

private:
  Vec3 _AA;
  Vec3 _BB;
};
