#include "RayTracer.hpp"

bool Render::ShadowRay(Vec3 &light, Hit &minHit, Vec3 &P, Vec3 &L) noexcept
{
    Ray shadowRay(P + minHit.normal * 0.001f, L);
    shadowRay.minHit = 0.001f;
    shadowRay.maxHit = norm(light - P) - 0.001f;

    if (this->bvh.intersect(shadowRay, minHit) == true)
        return true;
    return false;
}
