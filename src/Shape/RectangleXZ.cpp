#include "RayTracer.hpp"
#include "Object.hpp"

RectangleXZ::RectangleXZ(const libconfig::Setting& s){
    type = "RectangleXZ";
    x = scene::readVec2(s["x"]);
    z = scene::readVec2(s["z"]);
    y = (float)(s["y"]);
    color = scene::readVec3(s["color"]);
}    

AABB RectangleXZ::getObjectAABB() const
{
    return AABB(Vec3(x.x , y - EPS, z.x), Vec3(x.y, y + EPS, z.y));
} 

Vec3 RectangleXZ::getCentroid() const {
    return Vec3((this->x.x + this->x.y) / 2, this->y, (this->z.x + this->z.y) / 2);
}

bool RectangleXZ::intersect(Ray& ray, Hit& hit) const
{
    float t = (y - ray.origin.y) / ray.dir.y;
    float x = 0;
    float z = 0;

    if (t < ray.minHit || t > ray.maxHit)
        return false;

    x = ray.origin.x + t * ray.dir.x;
    z = ray.origin.z + t * ray.dir.z;

    if (x < this->x.x || x > this->x.y || z < this->z.x || z > this->z.y)
        return false;

    hit.t = t;
    hit.position = ray.origin + ray.dir * t;

    Vec3 normal(0, 1, 0);
    hit.normal = normal;
    hit.frontFace = true; 
    return true;
}