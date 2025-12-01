#pragma once

#include <iostream>
#include "CmdConfig.hpp"
#include "Scene.hpp"
#include "render.hpp"

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

namespace RayTracer {

    
    class RayTracer
    {
        public:
            RayTracer(const CmdConfig::config_t &config);
            void run();
    
        private:
            const CmdConfig::config_t &config;
            const scene::Scene scene;
            Render render;
    };
}