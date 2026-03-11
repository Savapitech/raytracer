#include "RayTracer.hpp"
#include "Object.hpp"

Cylinder::Cylinder(const libconfig::Setting &s)
{
    _type = "Cylinder";
    _pos = scene::readVec3(s["pos"]);
    _heigth = (float)s["heigth"];
    _radius = (float)s["radius"];
    _color = scene::readVec3(s["color"]);
    if (s.exists("dir"))
        _dir = scene::readVec3(s["dir"]);
    else
        _dir = {1,0,0};
}

AABB Cylinder::getObjectAABB() const 
{
    return AABB({ _pos.x - _radius, _pos.y - (_heigth / 2), _pos.z - _radius}, {_pos.x + _radius, _pos.y + (_heigth / 2), _pos.z + _radius});
}

Vec3 Cylinder::getCentroid() const
{
    return _pos;
}

Vec2 Cylinder::getUv(Vec3 &hitPos) const {
    return {0, 0};
}

bool Cylinder::intersect(Ray &ray, Hit &hit) const
{
    Vec3 center = _pos;
    float r = _radius;
    float h = _heigth;
    Vec3 v = _dir;
    Vec3 origin = ray.origin;
    Vec3 ray_dir = ray.origin - this->_pos;

    float a = (dot(ray_dir, ray_dir)) - ((dot(ray_dir, center))*(dot(ray_dir, center)));
    float b = 2.0 * ((dot(ray_dir, origin)) - (dot(ray_dir, center)) * (dot(origin, center)));
    float c = (dot(origin, origin)) - (dot(origin, center) * dot(origin, center)) - (r*r);

    float delta = b * b - 4.0f * a * c;
    if (delta < 0.0f)
        return false;
    float sqrtD = std::sqrt(delta);
    float inv2a = 0.5f / a;
    float t1 = (-b - sqrtD) * inv2a;
    float t2 = (-b + sqrtD) * inv2a;
    Vec3 p1 = ray.origin + ray.dir * t1;
    Vec3 p2 = ray.origin + ray.dir * t2;
    float m1 = dot(p1 - center, v);
    float m2 = dot(p2 - center, v);
    float halfHeight = h / 2.0;
    bool validT1 = (m1 >= -halfHeight && m1 <= halfHeight);
    bool validT2 = (m2 >= -halfHeight && m2 <= halfHeight);

    float t = std::numeric_limits<float>::infinity();
    if (validT1 && t1 > ray.minHit && t1 < ray.maxHit)
        t = t1;
    if (validT2 && t2 > ray.minHit && t2 < t)
        t = t2;

    if (!std::isfinite(t))
        return false;
    hit.t = t;
    hit.position = ray.origin + ray.dir * t;
    Vec3 outwardNormal = normalize(hit.position - this->_pos);
    hit.frontFace = dot(ray.dir, outwardNormal) < 0;
    hit.normal = hit.frontFace ? outwardNormal : -outwardNormal;
    return true;
}
