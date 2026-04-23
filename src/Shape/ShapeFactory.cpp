#include <libconfig.h++>
#include <memory>
#include <map>
#include <functional>

#include "Scene.hpp"

#include "Sphere.hpp"
#include "RectangleXZ.hpp"
#include "Cylinder.hpp"
#include "Triangle.hpp"
#include "Cube.hpp"

#include "SphereMarching.hpp"

using ShapeCtor = std::function<std::unique_ptr<IShape>(const libconfig::Setting &s)>;

std::map<std::string, ShapeCtor> shapeRegistry;

ShapeFactory::ShapeFactory()
{
    shapeRegistry["sphere"] = [](const libconfig::Setting& s) {
        return std::make_unique<Sphere>(s);
    };
    shapeRegistry["rectangleXZ"] = [](const libconfig::Setting& s) {
        return std::make_unique<RectangleXZ>(s);
    };
    shapeRegistry["triangle"] = [](const libconfig::Setting& s) {
        return std::make_unique<Triangle>(s);
    };
    shapeRegistry["cylinder"] = [](const libconfig::Setting& s) {
        return std::make_unique<Cylinder>(s);
    };
    shapeRegistry["cube"] = [](const libconfig::Setting& s) {
        return std::make_unique<Cube>(s);
    };
    shapeRegistry["sphereMarching"] = [](const libconfig::Setting& s) {
        return std::make_unique<SphereMarching>(s);
    };
}

std::unique_ptr<IShape> ShapeFactory::getShape(const libconfig::Setting &s)
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