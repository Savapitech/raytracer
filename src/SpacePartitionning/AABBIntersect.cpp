#include "RayTracer.hpp"

bool AABB::intersect(const Ray& r) const
{
    float tMin = r.minHit;
    float tMax = r.maxHit;

    for (int i = 0; i < 3; i++) {

        float invD = 1.0f / r.dir[i];
        float t0 = (min[i] - r.origin[i]) * invD;
        float t1 = (max[i] - r.origin[i]) * invD;

        if (invD < 0.0f)
            std::swap(t0, t1);

        tMin = std::max(tMin, t0);
        tMax = std::min(tMax, t1);

        if (tMax <= tMin)
            return false;
    }

    return true;
}