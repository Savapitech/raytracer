#pragma once
#include "IShape.hpp"

class Triangle : public AShape
{
public:
    Triangle(const libconfig::Setting& s);
    Triangle() = default;
    Triangle(Vec3 x, Vec3 y, Vec3 z);
    bool intersect(Ray& ray, Hit& hit) const noexcept override;
    AABB getObjectAABB() const noexcept override;
    Vec3 getCentroid() const noexcept override;
    Vec2 getUv(Vec3 &hitPos) const noexcept override;

    Vec3 x;
    Vec3 y;
    Vec3 z;
};
