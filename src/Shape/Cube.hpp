#pragma once
#include "IShape.hpp"

class Cube final : public AShape  
{
    public:
        bool intersect(Ray &ray, Hit &hit) const override;
        AABB getObjectAABB() const override;
        Vec3 getCentroid() const override;
        Vec2 getUv(Vec3 &hitPos) const override;
        Cube(const libconfig::Setting& s);
    private:
        float radius;
        Vec3 _AA;
        Vec3 _BB;
};
