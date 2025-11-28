#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "logger.hpp"
#include "Object.hpp"

namespace scene
{    
    typedef struct camera_s{
        Vec3 pos;
        Vec3 dir;
        double fov = 90;
    } camera_t;

    class Scene
    {
    public:
        Scene(const std::string &scene_path);
    private:
        const std::string &scene_path;
        camera_t cameraInfo;

        std::vector<std::shared_ptr<Object>> objects;
    };
} 
