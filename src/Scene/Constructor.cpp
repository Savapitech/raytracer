#include "Scene.hpp"
#include <libconfig.h++>
using namespace libconfig;

namespace scene {
    Vec3 readVec3(const Setting &s)
    {
        if (s.getLength() != 3)
            throw std::runtime_error("Expected array of 3 elements");
        return Vec3{ (float)s[0], (float)s[1], (float)s[2] };
    }

    
    camera_t readcam(const Setting &s)
    {
        camera_t camera;
        
        camera.fov = s.exists("fov") ? (float)s["fov"] : 90.0f;
        Log::Logger::debug("Camera Fov set");
        if (!s.exists("dir"))
            throw std::runtime_error("Missing 'dir' field in scene");
        camera.dir = readVec3(s["dir"]);
        Log::Logger::debug("Camera Dir set");
        if (!s.exists("pos"))
            throw std::runtime_error("Missing 'pos' field in scene");
        camera.dir = readVec3(s["pos"]);
        Log::Logger::debug("Camera Pos set");
        return camera;
    }

    

    Scene::Scene(const std::string &scene_path) : scene_path(scene_path) 
        {
            if (scene_path.empty() == true)
                throw std::invalid_argument("Error Scene Path is empty");
            Log::Logger::debug("Scene:" + scene_path);
            Config cfg;

            cfg.readFile(scene_path.data());
            Setting &root = cfg.getRoot();
            Setting &scene = root["scene"];
            this->cameraInfo = readcam(scene);
            readObject(scene, this->objects);
        }
}