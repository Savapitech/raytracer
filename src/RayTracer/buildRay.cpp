#include "bvh.hpp"

Vec3 direction(const scene::camera_t &camera, int x, int y){
    return Vec3(x - camera.width / 2, y - camera.height / 2, camera.distance);
}


Ray::Ray(const scene::camera_t &camera, int x, int y)
{
    this->origin = camera.pos;
    this->dir = normalize(direction(camera, x, y));
}