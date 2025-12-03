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
    materialRegistry["default"] = [](const libconfig::Setting& s) {
        return std::make_unique<Default>(s);
    };
}

#define MATERIAL "material"

std::unique_ptr<AMaterial> MaterialFactory::GetMaterial(const libconfig::Setting &s)
    {
        std::string type = ""; 
        if (s.exists(MATERIAL)) {
            const libconfig::Setting &s1 = s[MATERIAL];
            Log::Logger::debug("Material FOUND");
            if (!s1.exists("type"))
                throw std::invalid_argument("Need type to implement shape");
            Log::Logger::debug("Type FOUND");
            type = (std::string)s1["type"];
            Log::Logger::debug("Type load");
        }
        else
            type = "default";
        
        auto fcntShape = materialRegistry.at(type);
        if (type == "default")
            return fcntShape(s);
        Log::Logger::debug("Map use");

        if (fcntShape == nullptr){
            throw std::invalid_argument("Type: " + type + " didn't exist.");
        }
        return fcntShape(s[MATERIAL]);
    }