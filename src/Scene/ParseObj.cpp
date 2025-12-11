#include <libconfig.h++>
#include <fstream>
#include <sstream>
#include <string>

#include "Scene.hpp"

using namespace libconfig;

scene::Obj::Obj(std::string path)
{
    /*PARSE OBJ*/
}

void scene::Scene::insertObjInObjects(const Setting &s, std::vector<std::unique_ptr<Object>> &objects)
{
    if (!s.exists("obj")){
        Log::Logger::warning("No Obj");
        return;    
    }
    Log::Logger::info("Parse Obj");
    //Obj newObj("./Exemple/pedkzlpvxb-LowPolySpaceShip/SpaceShip.obj");
    auto &src = newObj.getObjects();
    for (auto &obj : src) {
        if (obj) {
            objects.push_back(std::move(obj));
        }
    }
    Log::Logger::info("End of parsing Obj");
}