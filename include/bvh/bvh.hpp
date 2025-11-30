#pragma once

#include "Scene.hpp"

class Ray
{
    public:
        Vec3 origin;
        Vec3 dir;
        float minHit = 0.001f;
        float maxHit = 1e30f;

        Ray(const scene::camera_t &camera, int x, int y);
        Ray(const Vec3& origin, const Vec3& direction);
};

class Hit {
    public:
        float t = 1e30f;
        Vec3 position;
        Vec3 normal;
        Object *object = nullptr;

        bool hit() const { return object != nullptr;}
};