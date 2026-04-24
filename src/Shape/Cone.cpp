#include "Cone.hpp"

#include "Ray.hpp"
#include <cmath>
#include "Object.hpp"

Cone::Cone(const libconfig::Setting &s)
{
    _type = "Cone";
    _pos = scene::readVec3(s["pos"]);
    _radius = (float)s["radius"];
    _height = (float)s["height"];
    _color = scene::readVec3(s["color"]);
    if (s.exists("dir"))
        _dir = scene::readVec3(s["dir"]);
    else
        _dir = {1,0,0};
}

AABB Cone::getObjectAABB() const noexcept
{
    return AABB({_pos.x - _radius, _pos.y - _radius, _pos.z - _radius}, {_pos.x + _radius, _pos.y + _radius, _pos.z + _radius});
}

Vec3 Cone::getCentroid() const noexcept
{
    return _pos;
}

Vec2 Cone::getUv(Vec3 &) const noexcept
{
    return {0, 0};
}

bool Cone::intersect(Ray &ray, Hit &hit) const noexcept 
{
    Vec3 center = _pos;
    Vec3 v = normalize(_dir);
    Vec3 origin = ray.origin - _pos;
    Vec3 rayDir = ray.dir;
    float r = _radius;
    float h = _height;
    float alpha = (_radius / _height);
    float k = std::tan(alpha);

    float a = (dot(rayDir, rayDir) - ((1.0f + (k * k)) * (dot(rayDir, v) * dot(rayDir, v))));
    float b = 2.0 * (dot(rayDir, origin) - ((1.0f + (k * k)) * (dot(rayDir, v) * dot(origin, v))));
    float c = (dot(origin, origin) - ((1.0f + (k * k)) * (dot(origin, v) * dot(origin, v))));

    float delta = b * b - 4.0f * a * c;
    if (delta < 0.0f || fabs(a) < 1e-6f)
        return false;
    float sqrtD = std::sqrt(delta);
    float inv2a = 0.5f / a;
    float t1 = (-b - sqrtD) * inv2a;
    float t2 = (-b + sqrtD) * inv2a;
    Vec3 p1 = ray.origin + ray.dir * t1;
    Vec3 p2 = ray.origin + ray.dir * t2;
    float m1 = dot(p1 - center, v);
    float m2 = dot(p2 - center, v);
    float halfHeight = h / 2.0f;
    
    bool validT1 = (m1 >= -halfHeight && m1 <= halfHeight);
    bool validT2 = (m2 >= -halfHeight && m2 <= halfHeight);

    float t = std::numeric_limits<float>::infinity();
    Vec3 normal_at_hit;

    if (validT1 && t1 > ray.minHit && t1 < ray.maxHit) {
        t = t1;
        normal_at_hit = normalize((ray.origin + ray.dir * t) - this->_pos - (v * m1));
    }
    if (validT2 && t2 > ray.minHit && t2 < t) {
        t = t2;
        normal_at_hit = normalize((ray.origin + ray.dir * t) - this->_pos - (v * m2));
    }

    Vec3 top_center = center + v * halfHeight;
    Vec3 bottom_center = center - v * halfHeight;

    auto testCap = [&](const Vec3& cap_center, const Vec3& cap_normal) {
        float denom = dot(rayDir, cap_normal);
        if (std::abs(denom) > 1e-6f) {
            float t_cap = dot(cap_center - ray.origin, cap_normal) / denom;
            if (t_cap > ray.minHit && t_cap < t) {
                Vec3 hit_point = ray.origin + rayDir * t_cap;
                Vec3 vector_to_center = hit_point - cap_center;

                if (dot(vector_to_center, vector_to_center) <= (r * r)) {
                    t = t_cap;
                    normal_at_hit = cap_normal;
                }
            }
        }
    };

    testCap(top_center, v);
    testCap(bottom_center, -v);

    if (!std::isfinite(t))
        return false;

    hit.t = t;
    hit.position = ray.origin + ray.dir * t;
    hit.frontFace = dot(ray.dir, normal_at_hit) < 0;
    hit.normal = hit.frontFace ? normal_at_hit : -normal_at_hit;
    
    return true;
}