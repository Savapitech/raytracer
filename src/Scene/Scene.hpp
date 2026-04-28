#pragma once

#include <memory>
#include <vector>
#include <libconfig.h++>

#include "MaterialRegistry.hpp"
#include "ILight.hpp"
#include "Object.hpp"
#include "BVH.hpp"

#define WIDTH 1920
#define HEIGHT 1080

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
            int width = WIDTH;
            int height = HEIGHT;

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
            Obj(const std::string & path, const libconfig::Setting &s);
            std::vector<std::unique_ptr<Object>> &getObjects(void) {return _objects;}
            void parseMtl(const std::string &path);
        private:
            MaterialRegistry _materialRegistry;
            std::vector<Vec3> _vertices;
            std::vector<std::unique_ptr<Object>> _objects;
    };

    class Scene
    {
    public:
        Scene(const std::string &scene_path);

        const std::vector<std::unique_ptr<Object>> &getObjects(void) const{return _objects;}
        const std::vector<std::unique_ptr<ILight>> &getLights(void) {return _lights;}

        const Camera &getCamera(void) const{return _cameraInfo;}
        void updateCamera(Vec3 pos){
            _cameraInfo.pos.x = pos.x;
            _cameraInfo.pos.y = pos.y;
            _cameraInfo.pos.z = pos.z;
        }
        void updateCameraDir(Vec3 dir){
            _cameraInfo.dir = dir;
            _cameraInfo.setupCam();
        }
        
    private:
        Factory _factory;

        void insertObjInObjects(const libconfig::Setting &s, std::vector<std::unique_ptr<Object>> &objects);
        void readObject(const libconfig::Setting &s, std::vector<std::unique_ptr<Object>> &objects);
        Camera _cameraInfo;
        std::vector<std::unique_ptr<Object>> _objects;
        std::vector<std::unique_ptr<ILight>> _lights;
    };
} 
