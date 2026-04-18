#pragma once

#include "math3d.hpp"

class AABB
{
    public:
        Vec3 min;
        Vec3 max;
        AABB(Vec3 a, Vec3 b)
        {
            min = a;
            max = b;
        };
        AABB() = default;

        float surfaceArea() const
        {
            Vec3 d = max - min;

            if (d.x < 0 || d.y < 0 || d.z < 0)
                return 0.0f;
            return 2.0f * (d.x * d.y + d.y * d.z + d.z * d.x);
        }
        bool intersect(const Ray &r) const;
        void normalize();
};
