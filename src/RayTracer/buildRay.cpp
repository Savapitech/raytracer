#include "Scene.hpp"
#include "RayTracer.hpp"

Ray::Ray(const scene::Camera &camera, int x, int y)
{
    float aspect = float(camera.width) / float(camera.height);
    float scale  = tanf((camera.fov * 0.5f) * (M_PI / 180.0f));

    float dirx = (2 * (x + 0.5f) / camera.width  - 1) * aspect * scale;
    float diry = (1 - 2 * (y + 0.5f) / camera.height) * scale;

    this->dir = normalize(camera.forward + camera.right * dirx + camera.up * diry);
    this->origin = camera.pos;
}

Ray::Ray(const Vec3& origin, const Vec3& direction)
{
    this->origin = origin;
    this->dir = normalize(direction);
    this->minHit = 0.001f;
    this->maxHit = 1e30f;
}