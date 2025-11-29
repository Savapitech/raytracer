#include <libconfig.h++>
#include <memory>
#include <map>
#include <functional>

#include "Scene.hpp"

using ShapeCtor = std::function<std::unique_ptr<AShape>(const libconfig::Setting &s)>;

std::map<std::string, ShapeCtor> shapeRegistry;

ShapeFactory::ShapeFactory()
{
    shapeRegistry["sphere"] = [](const libconfig::Setting& s) {
        return std::make_unique<Sphere>(
            scene::readVec3(s["pos"]),
            (float)s["radius"]
        );
    };
}


std::unique_ptr<AShape> ShapeFactory::GetShape(const libconfig::Setting &s)
    {
        if (!s.exists("type"))
            throw std::invalid_argument("Need type to implement shape");

        std::string type = s["type"];
        auto fcntShape = shapeRegistry.at(type);
        if (fcntShape == nullptr){
            throw std::invalid_argument("Type: " + type + " didn't exist.");
        }
        return fcntShape(s);
    }