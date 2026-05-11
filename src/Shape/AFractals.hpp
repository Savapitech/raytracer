#pragma once
#include "IShape.hpp"
#include "Ray.hpp"

class AFractals : public AShape {
public:
  AFractals() = default;
  virtual ~AFractals() = default;
  virtual float evaluateSDF(const Vec3 &p) const = 0;
  bool intersect(Ray &ray, Hit &hit) const override;
  Vec2 getUv(Vec3 &hitPos) const override;
  Vec3 calculateNormale(const Vec3 &p) const;

protected:
  int _maxIt = 500;
  float _collisionEpsilon = 0.0001f;
  float _escapeDist = 50.0f;
};
