#pragma once
#include "IShape.hpp"

class RectangleXZ : public AShape
{
public:
    RectangleXZ(const libconfig::Setting& s);
    bool intersect(Ray& ray, Hit& hit) const override;
    AABB getObjectAABB() const override;
    Vec3 getCentroid() const override;
    Vec2 getUv(Vec3 &hitPos) const override;
    
    Vec2 x;
    float y;
    Vec2 z;
};
