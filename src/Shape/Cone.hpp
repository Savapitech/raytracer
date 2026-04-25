#pragma once
#include "IShape.hpp"


class Cone : public AShape
{
    public:
        Cone(const libconfig::Setting &s);
        bool intersect(Ray &ray, Hit &hit) const noexcept override;
        AABB getObjectAABB() const noexcept override;
        Vec3 getCentroid() const noexcept override;
        Vec2 getUv(Vec3 &hitPos) const noexcept override;
    private:
        float _radius;
        float _height;
        Vec3 _dir;
};
