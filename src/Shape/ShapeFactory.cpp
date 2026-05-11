#include <functional>
#include <libconfig.h++>
#include <map>
#include <memory>

#include "Scene.hpp"

#include "Cone.hpp"
#include "ConeInf.hpp"
#include "Cube.hpp"
#include "Cylinder.hpp"
#include "CylinderInf.hpp"
#include "RectangleXZ.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

#include "Plane.hpp"

#include "Mandelbox.hpp"
#include "Mandelbulb.hpp"
#include "MobiusStrip.hpp"
#include "SphereMarching.hpp"

using ShapeCtor =
    std::function<std::unique_ptr<IShape>(const libconfig::Setting &s)>;

std::map<std::string, ShapeCtor> shapeRegistry;

ShapeFactory::ShapeFactory() {
  shapeRegistry["sphere"] = [](const libconfig::Setting &s) {
    return std::make_unique<Sphere>(s);
  };
  shapeRegistry["rectangleXZ"] = [](const libconfig::Setting &s) {
    return std::make_unique<RectangleXZ>(s);
  };
  shapeRegistry["triangle"] = [](const libconfig::Setting &s) {
    return std::make_unique<Triangle>(s);
  };
  shapeRegistry["cylinder"] = [](const libconfig::Setting &s) {
    return std::make_unique<Cylinder>(s);
  };
  shapeRegistry["CylinderInf"] = [](const libconfig::Setting &s) {
    return std::make_unique<CylinderInf>(s);
  };
  shapeRegistry["cube"] = [](const libconfig::Setting &s) {
    return std::make_unique<Cube>(s);
  };
  shapeRegistry["Cone"] = [](const libconfig::Setting &s) {
    return std::make_unique<Cone>(s);
  };
  shapeRegistry["ConeInf"] = [](const libconfig::Setting &s) {
    return std::make_unique<ConeInf>(s);
  };
  shapeRegistry["sphereMarching"] = [](const libconfig::Setting &s) {
    return std::make_unique<SphereMarching>(s);
  };
  shapeRegistry["Mandelbulb"] = [](const libconfig::Setting &s) {
    return std::make_unique<Mandelbulb>(s);
  };
  shapeRegistry["plane"] = [](const libconfig::Setting &s) {
    return std::make_unique<Plane>(s);
  };
  shapeRegistry["Mandelbox"] = [](const libconfig::Setting &s) {
    return std::make_unique<Mandelbox>(s);
  };
  shapeRegistry["MobiusStrip"] = [](const libconfig::Setting &s) {
    return std::make_unique<MobiusStrip>(s);
  };
}

std::unique_ptr<IShape> ShapeFactory::getShape(const libconfig::Setting &s) {
  if (!s.exists("type"))
    throw std::invalid_argument("Need type to implement shape");

  std::string type = s["type"];
  auto fcntShape = shapeRegistry.at(type);
  if (fcntShape == nullptr) {
    throw std::invalid_argument("Type: " + type + " didn't exist.");
  }
  return fcntShape(s);
}