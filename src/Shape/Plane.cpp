#include "Ray.hpp"
#include "Plane.hpp"
#include "Object.hpp"

Plane::Plane(const libconfig::Setting& s){
    _type = "plane";
    _pos = scene::readVec3(s["pos"]);
    _normale = scene::readVec3(s["normal"]);
    _color = scene::readVec3(s["color"]);
}    

AABB Plane::getObjectAABB() const noexcept
{
    float inf = std::numeric_limits<float>::infinity();
    return AABB(Vec3(-inf, -inf, -inf), Vec3(inf, inf, inf));
}

Vec3 Plane::getCentroid() const noexcept
{
    return _pos;
}

Vec2 Plane::getUv(Vec3 &hitPos) const noexcept
{
    Vec3 up(0.0f, 1.0f, 0.0f);

    if (std::abs(_normale.y) > 0.999f) {
        up = Vec3(1.0f, 0.0f, 0.0f);
    }

    Vec3 u_axis = normalize(cross(up, _normale));
    Vec3 v_axis = cross(_normale, u_axis);

    Vec3 p = hitPos - _pos;

    float u = dot(p, u_axis);
    float v = dot(p, v_axis);

    return {u, v};
}

bool Plane::intersect(Ray& ray, Hit& hit) const noexcept
{
    float denom = dot(_normale, ray.dir);

    if (std::abs(denom) > EPS) {
        Vec3 dirRayToPlan = _pos - ray.origin;
        float t = dot(dirRayToPlan, _normale) / denom; 

        if (t > EPS && t < hit.t) {
            hit.t = t;
            hit.position = ray.origin + ray.dir * t;
            hit.normal = _normale;
            if (denom > 0) 
                hit.normal = -_normale;

            return true;
        }
    }

    return false;
}