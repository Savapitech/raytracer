#include "RayTracer.hpp"
#include "Object.hpp"
#include <cmath>

Sphere::Sphere(const libconfig::Setting& s){
    type = "Sphere";
    radius = (float)s["radius"];
    pos = scene::readVec3(s["pos"]);
    color = scene::readVec3(s["color"]);
}    

AABB Sphere::getObjectAABB() const
{
    return {{ pos.x - radius, pos.y - radius, pos.z - radius }, { pos.x + radius, pos.y + radius, pos.z + radius }};
} 

Vec3 Sphere::getCentroid() const {
    return this->pos;
}

Vec2 Sphere::getUv(Vec3 &hitPos) const {
    Vec2 uv;
    Vec3 localP = (hitPos - this->pos) / this->radius;

    uv.x = 0.5 + (atan2(localP.z, localP.x) / (2 * M_PI));
    uv.y = 0.5 + (asin(localP.y) / M_PI);
    return uv;
}

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
    Vec3 outwardNormal = normalize(hit.position - this->pos);
    hit.frontFace = dot(ray.dir, outwardNormal) < 0;
    hit.normal = hit.frontFace ? outwardNormal : -outwardNormal;
    return true;
}