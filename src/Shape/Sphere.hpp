#pragma once
#include "IShape.hpp"

class Sphere final : public AShape  
{
    public:
        bool intersect(Ray &ray, Hit &hit) const noexcept override;
        AABB getObjectAABB() const noexcept override;
        Vec3 getCentroid() const noexcept override;
        Vec2 getUv(Vec3 &hitPos) const noexcept override;
        Sphere(const libconfig::Setting& s);
    private:
        float _radius;
};
