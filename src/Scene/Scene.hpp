#pragma once

#include <memory>
#include <vector>
#include <libconfig.h++>

#include "ILight.hpp"
#include "Object.hpp"
#include "BVH.hpp"

#define DEG_TO_RAD(angle) ((angle) * (M_PI / 180.0))

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
            float fov = 30;
            double distance = 540;
            int width = 1920;
            int height = 1080;

            void setupCam();

    };

    Camera readcam(const libconfig::Setting &s);

    class Factory{
        public:
            std::unique_ptr<Object> getObject(const libconfig::Setting &s);
            //void GetILight();
    };

    class Obj{
        public:
            Obj(std::string path, const libconfig::Setting &s);
            std::vector<std::unique_ptr<Object>> &getObjects(void) {return objects;}
        private:
            std::vector<Vec3> vertices;
            std::vector<std::unique_ptr<Object>> objects;
    };

    class Scene
    {
    public:
        Scene(const std::string &scene_path);

        const std::vector<std::unique_ptr<Object>> &getObjects(void) const{return objects;}
        const std::vector<std::unique_ptr<ILight>> &getLights(void) {return _lights;}
        
        const Camera &getCamera(void) const{return cameraInfo;}
        void updateCamera(Vec3 pos){
            cameraInfo.pos.x = pos.x;
            cameraInfo.pos.y = pos.y; 
            cameraInfo.pos.z = pos.z;
        }
        void updateCameraDir(Vec3 dir){
            cameraInfo.dir = dir;
            cameraInfo.setupCam();
        }
        
    private:
        Factory factory;

        double deapth;
        void insertObjInObjects(const libconfig::Setting &s, std::vector<std::unique_ptr<Object>> &objects);
        void readObject(const libconfig::Setting &s, std::vector<std::unique_ptr<Object>> &objects);
        Camera cameraInfo;
        std::vector<std::unique_ptr<Object>> objects; // send
        std::vector<std::unique_ptr<ILight>> _lights;
    };
} 
