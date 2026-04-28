#include "ILight.hpp"
#include "Scene.hpp"
#include <memory>
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

    std::unique_ptr<Object> Factory::getObject(const Setting &s)
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
            objects.push_back(_factory.getObject(list[i]));
            Log::Logger::debug("Objects add: " + std::to_string(count));
            Log::Logger::debug("Objects type: Shape:" + objects.back()->getShape()->getType());
            if (objects.back()->getMaterial() != nullptr)
                Log::Logger::debug("Objects type: Material:" + objects.back()->getMaterial()->type);
        }
        insertObjInObjects(s, objects);
    }

    void Scene::readLight(const Setting &s, std::vector<std::unique_ptr<ILight>> &lights)
    {
        if (!s.exists("lights"))
            throw std::runtime_error("'lights' field is missing in scene");
    
        Setting &list = s["lights"];
    
        if (!list.isList())
            throw std::runtime_error("'lights' must be a list ( ... )");
    
        int count = list.getLength();
        Log::Logger::debug("lights size: " + std::to_string(count));
        
        for (int i = 0; i < count; i++){
            lights.push_back(_lightFactory.getLight(list[i]));
            Log::Logger::debug("Lights add: " + std::to_string(count));
        }
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
            
            if (scene.exists("background"))
                Material::textureManager.uploadTexture((std::string)scene["background"]);

            _cameraInfo = readcam(scene);
            readObject(scene, _objects);
            readLight(scene, _lights);
            //this->_lights.push_back(std::make_unique<AreaLight>(_cameraInfo.pos,   Vec3{1.0f, 1.0f, 1.0f}, 8));
            //this->_lights.push_back(std::make_unique<AreaLight>(Vec3{0, 80, 10},        Vec3{1.0f, 1.0f, 1.0f}, 8));
            //this->_lights.push_back(std::make_unique<AreaLight>(Vec3{30, 40, -20},      Vec3{1.0f, 1.0f, 1.0f}, 8));
            //this->_lights.push_back(std::make_unique<AreaLight>(Vec3{-30, 20, 30},      Vec3{10.00f, 10.0f, 10.00f}, 8)); 
        }
}
