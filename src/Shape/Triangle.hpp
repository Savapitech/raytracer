#pragma once
#include "IShape.hpp"

class Triangle : public AShape
{
public:
    Triangle(const libconfig::Setting& s);
    Triangle() = default;
    Triangle(Vec3 x, Vec3 y, Vec3 z);
    bool intersect(Ray& ray, Hit& hit) const override;
    AABB getObjectAABB() const override;
    Vec3 getCentroid() const override;
    Vec2 getUv(Vec3 &hitPos) const override;

    Vec3 x;
    Vec3 y;
    Vec3 z;
};
