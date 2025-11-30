#include "bvh.hpp"
#include "Object.hpp"


bool Sphere::intersect(Ray &ray, Hit &hit) const
{
    Vec3 u  = ray.dir;
    Vec3 oc = ray.origin - this->pos;
    float r = this->radius;

    float a = dot(u, u);               
    float b = 2.0f * dot(u, oc);
    float c = dot(oc, oc) - r * r;

    float delta = b * b - 4.0f * a * c;
    if (delta < 0.0f)
        return false;

    float sqrtD = std::sqrt(delta);
    float inv2a = 0.5f / a;

    float t1 = (-b - sqrtD) * inv2a;
    float t2 = (-b + sqrtD) * inv2a;

    float t = std::numeric_limits<float>::infinity();

    if (t1 > ray.minHit && t1 < ray.maxHit)
        t = t1;
    if (t2 > ray.minHit && t2 < t)
        t = t2;

    if (!std::isfinite(t))
        return false;

    hit.t = t;
    hit.position = ray.origin + u * t;
    hit.normal   = normalize(hit.position - this->pos);
    return true;
}