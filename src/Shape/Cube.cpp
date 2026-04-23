#include <cmath>

#include "Cube.hpp"
#include "Ray.hpp"
#include "Object.hpp"

Cube::Cube(const libconfig::Setting& s){
    _type = "Cube";
    _pos = scene::readVec3(s["pos"]);
    _color = scene::readVec3(s["color"]);
    float size = (float)s["size"];
    _AA = _pos - size;
    _BB = _pos + size;
}    

AABB Cube::getObjectAABB() const noexcept 
{
    return {_AA, _BB};
} 

Vec3 Cube::getCentroid() const noexcept
{
    return this->_pos;
}

Vec2 Cube::getUv(Vec3 &) const noexcept
{
    return {0, 0};
}


bool Cube::intersect(Ray &ray, Hit &hit) const noexcept
{
    float tMin = ray.minHit;
    float tMax = ray.maxHit;
    
    int hitAxis = -1;

    for (int i = 0; i < 3; i++) {
        float t0 = (_AA[i] - ray.origin[i]) * ray.invDir[i];
        float t1 = (_BB[i] - ray.origin[i]) * ray.invDir[i];
   
        bool isNearT0 = t0 < t1;
        float tNear = isNearT0 ? t0 : t1;
        float tFar  = isNearT0 ? t1 : t0;

        tMax = std::min(tMax, tFar);

        if (tNear > tMin) {
            tMin = tNear;
            hitAxis = i;
        }
        
        tMax = std::min(tMax, tFar);

        if (tMax <= tMin)
            return false;
    }

    if (tMin < hit.t) {
        hit.t = tMin;
        hit.position = ray.origin + ray.dir * hit.t;
        
        Vec3 normal(0.0f, 0.0f, 0.0f);
        if (hitAxis != -1) {
           normal[hitAxis] = (ray.dir[hitAxis] < 0.0f) ? 1.0f : -1.0f;
        }
        hit.normal = normal;    
        return true;
    }

    return false;
}