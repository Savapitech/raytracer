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

    std::unique_ptr<Object> Factory::GetObject(const Setting &s)
    {
        if (!s.exists("shape"))
            throw std::invalid_argument("Object need a shape");
        if (!s.exists("matrial"))
            return std::make_unique<Object>(s, false);
        return std::make_unique<Object>(s, true);
    }

    //void Scene::readObject(const Setting &s, std::vector<std::unique_ptr<Object>> &objects)
    //{
    //    if (!s.exists("objects") && s["objects"].isGroup())
    //         throw std::runtime_error("'objects' field in scene must be an array or existed");
    //    Log::Logger::debug("Objects size: " + std::to_string(s["objects"].getLength()));
    //    for (int i = 0; i != s["objects"].getLength(); i++)
    //        objects.push_back(factory.GetObject(s[i]));
    //}
    void Scene::readObject(const Setting &s, std::vector<std::unique_ptr<Object>> &objects)
    {
        if (!s.exists("objects"))
            throw std::runtime_error("'objects' field is missing in scene");
    
        Setting &list = s["objects"];
    
        if (!list.isList())
            throw std::runtime_error("'objects' must be a list ( ... )");
    
        int count = list.getLength();
        Log::Logger::debug("Objects size: " + std::to_string(count));
    
        for (int i = 0; i < count; i++){
            objects.push_back(factory.GetObject(list[i]));
            Log::Logger::debug("Objects add: " + std::to_string(count));
        }
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