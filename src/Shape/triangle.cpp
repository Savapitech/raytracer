#include "RayTracer.hpp"
#include "Object.hpp"
#include <cmath>
#include <limits>

Triangle::Triangle(const libconfig::Setting& s)
{
    type = "Triangle";
    x = scene::readVec3(s["x"]);
    y = scene::readVec3(s["y"]);
    z = scene::readVec3(s["z"]);
    color = scene::readVec3(s["color"]);
}

Triangle::Triangle(Vec3 x, Vec3 y, Vec3 z)
{
    type = "Triangle";
    this->x = x;
    this->y = y;
    this->z = z;
    this->color = {255, 0, 255};
}

Vec2 Triangle::getUv(Vec3 &hitPos) const {
    return {0, 0};
}


AABB Triangle::getObjectAABB() const
{
    Vec3 minP(
        std::min({x.x, y.x, z.x}),
        std::min({x.y, y.y, z.y}),
        std::min({x.z, y.z, z.z})
    );
    Vec3 maxP(
        std::max({x.x, y.x, z.x}),
        std::max({x.y, y.y, z.y}),
        std::max({x.z, y.z, z.z})
    );
    return { minP, maxP };
}

Vec3 Triangle::getCentroid() const
{
    return (x + y + z) / 3.0f;
}

bool Triangle::intersect(Ray &ray, Hit &hit) const
{
    Vec3 edge1 = y - x;
    Vec3 edge2 = z - x;

    Vec3 pvec = cross(ray.dir, edge2);
    float det = dot(edge1, pvec);

    if (std::fabs(det) < EPS)
        return false;

    float invDet = 1.0f / det;
    Vec3 tvec = ray.origin - x;
    float u = dot(tvec, pvec) * invDet;

    if (u < 0.0f || u > 1.0f)
        return false;

    Vec3 qvec = cross(tvec, edge1);
    float v = dot(ray.dir, qvec) * invDet;
    if (v < 0.0f || u + v > 1.0f)
        return false;

    float t = dot(edge2, qvec) * invDet;

    if (t < ray.minHit || t > ray.maxHit)
        return false;
    hit.t = t;
    hit.position = ray.origin + ray.dir * t;

    Vec3 outwardNormal = normalize(cross(edge1, edge2));

    hit.frontFace = dot(ray.dir, outwardNormal) < 0;
    hit.normal = hit.frontFace ? outwardNormal : -outwardNormal;

    return true;
}
