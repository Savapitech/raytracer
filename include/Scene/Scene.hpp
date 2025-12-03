#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <libconfig.h++>

#include "Object.hpp"
#include "bvh.hpp"

namespace scene
{   
    Vec2 readVec2(const libconfig::Setting &s);
    Vec3 readVec3(const libconfig::Setting &s);

    

    class Camera{
        public:
            Vec3 pos;
            Vec3 dir;
            Vec3 forward;
            Vec3 right;
            Vec3 up;
            float fov = 90;
            double distance = 540;
            int width = 1920;
            int height = 1080;

            void setupCam();

    };

    Camera readcam(const libconfig::Setting &s);

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
        const Camera &getCamera(void) const{return cameraInfo;}
    private:
        Factory factory;
        
        void readObject(const libconfig::Setting &s, std::vector<std::unique_ptr<Object>> &objects);
        Camera cameraInfo;
        std::vector<std::unique_ptr<Object>> objects;
    };
} 
