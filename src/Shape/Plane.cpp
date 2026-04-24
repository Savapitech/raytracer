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
    return this->_pos;
}

Vec2 Plane::getUv(Vec3 &) const noexcept
{
    
}

bool Plane::intersect(Ray& ray, Hit& hit) const noexcept
{
    float denom = dot(_normale, ray.dir);

    if (std::abs(denom) > EPS) {
        Vec3 dirRayToPlan = this->_pos - ray.origin;
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