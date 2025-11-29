#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <libconfig.h++>
#include "logger.hpp"

#include "math3d.hpp"
#include "Object.hpp"

namespace scene
{   
    Vec3 readVec3(const libconfig::Setting &s);

    typedef struct camera_s{
        Vec3 pos;
        Vec3 dir;
        double fov = 90;
    } camera_t;

    class Factory{
        public:
            std::unique_ptr<Object> GetObject(const libconfig::Setting &s);
            //void GetILight();
    };

    class Scene
    {
    public:
        Scene(const std::string &scene_path);
        const std::vector<std::unique_ptr<Object>> &getObject(void) const{return objects;}
    private:
        Factory factory;
        void readObject(const libconfig::Setting &s, std::vector<std::unique_ptr<Object>> &objects);
        camera_t cameraInfo;

        std::vector<std::unique_ptr<Object>> objects;
    };
} 
