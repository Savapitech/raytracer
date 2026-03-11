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
    float max;
    if (_radius > _heigth / 2.0f)
        max = _radius;
    else
        max = _heigth / 2.0f;

    return AABB({ _pos.x - max, _pos.y - max, _pos.z - max}, {_pos.x + max, _pos.y + max, _pos.z + max});
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
    Vec3 origin = ray.origin - this->_pos;
    Vec3 ray_dir = ray.dir;

    float a = (dot(ray_dir, ray_dir)) - ((dot(ray_dir, v))*(dot(ray_dir, v)));
    float b = 2.0 * ((dot(ray_dir, origin)) - (dot(ray_dir, v)) * (dot(origin, v)));
    float c = (dot(origin, origin)) - (dot(origin, v) * dot(origin, v)) - (r*r);

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
    float m = 0.0f;
    if (validT1 && t1 > ray.minHit && t1 < ray.maxHit){
        t = t1;
        m = m1;
    }
    if (validT2 && t2 > ray.minHit && t2 < t){
        t = t2;
        m = m2;
    }

    if (!std::isfinite(t))
        return false;
    hit.t = t;
    hit.position = ray.origin + ray.dir * t;
    Vec3 outwardNormal = normalize(hit.position - this->_pos - (v*m));
    hit.frontFace = dot(ray.dir, outwardNormal) < 0;
    hit.normal = hit.frontFace ? outwardNormal : -outwardNormal;
    return true;
}
