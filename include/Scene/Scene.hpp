#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <libconfig.h++>

#include "Object.hpp"
#include "bvh.hpp"

namespace scene
{   
    Vec3 readVec3(const libconfig::Setting &s);

    typedef struct camera_s{
        Vec3 pos;
        Vec3 dir;
        float fov = 90;
        double distance = 540;
        int width = 1920;
        int height = 1080;

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

        const std::vector<std::unique_ptr<Object>> &getObjects(void) const{return objects;}
        const camera_t &getCamera(void) const{return cameraInfo;}
    private:
        Factory factory;
        
        void readObject(const libconfig::Setting &s, std::vector<std::unique_ptr<Object>> &objects);
        camera_t cameraInfo;
        std::vector<std::unique_ptr<Object>> objects;
    };
} 
