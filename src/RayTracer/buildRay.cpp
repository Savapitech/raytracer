#include "bvh.hpp"

Vec3 direction(const scene::camera_t &camera, int x, int y){
    return Vec3(x - camera.width / 2, y - camera.height / 2, camera.distance);
}


Ray::Ray(const scene::camera_t &camera, int x, int y)
{
    this->origin = camera.pos;
    this->dir = normalize(direction(camera, x, y));
}

Ray::Ray(const Vec3& origin, const Vec3& direction)
{
    this->origin = origin;
    this->dir = normalize(direction);
    this->minHit = 0.001f;
    this->maxHit = 1e30f;
}