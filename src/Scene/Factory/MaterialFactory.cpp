#include <libconfig.h++>
#include <memory>
#include <map>
#include <functional>

#include "Scene.hpp"

using MaterialCtor = std::function<std::unique_ptr<AMaterial>(const libconfig::Setting &s)>;

std::map<std::string, MaterialCtor> materialRegistry;

MaterialFactory::MaterialFactory()
{
    materialRegistry["mirror"] = [](const libconfig::Setting& s) {
        return std::make_unique<Mirror>(s);
    };
}

Mirror::Mirror(const libconfig::Setting& s){
    this->type = "Mirror";
    this->reflect = (float)s["reflect"];
    this->color = {255, 255, 255};
}            


std::unique_ptr<AMaterial> MaterialFactory::GetMaterial(const libconfig::Setting &s)
    {
        if (!s.exists("type"))
            throw std::invalid_argument("Need type to implement shape");

        std::string type = s["type"];
        auto fcntShape = materialRegistry.at(type);
        if (fcntShape == nullptr){
            throw std::invalid_argument("Type: " + type + " didn't exist.");
        }
        return fcntShape(s);
    }