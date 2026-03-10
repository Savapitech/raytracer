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

    Vec2 readVec2(const Setting &s)
    {
        if (s.getLength() != 2)
            throw std::runtime_error("Expected array of 3 elements");
        return Vec2{(float)s[0], (float)s[1]};
    }

    std::unique_ptr<Object> Factory::GetObject(const Setting &s)
    {
        if (!s.exists("shape"))
            throw std::invalid_argument("Object need a shape");
        return std::make_unique<Object>(s);
    }

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
            Log::Logger::debug("Objects type: Shape:" + objects.back()->shape->getType());
            if (objects.back()->material != nullptr)
                Log::Logger::debug("Objects type: Material:" + objects.back()->material->type);
        }
        this->insertObjInObjects(s, objects);
    }

    Scene::Scene(const std::string &scene_path)
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