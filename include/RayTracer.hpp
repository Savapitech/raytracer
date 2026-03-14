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
        float invDir[3];
        float minHit = 0.001f;
        float maxHit = 1e30f;

        Ray() = default;
        Ray(const scene::Camera &camera, float x, float y);
        Ray(const Vec3& origin, const Vec3& direction);
};

class Hit {
    public:
        float t = 1e30f;
        Vec3 position;
        Vec3 normal;
        /*Idx into the bvh*/
        int ObjectIdx = -1;
        bool frontFace = false;

  //      bool hit() const { return object != nullptr;}
};

namespace RayTracer {

    
    class RayTracer
    {
        public:
            RayTracer(const CmdConfig::config_t &config);
            void run();
    
        private:
            const CmdConfig::config_t &config;
            scene::Scene scene;
            Render render;
    };
}