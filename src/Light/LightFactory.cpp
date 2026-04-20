#include "ILight.hpp"
#include <libconfig.h++>
#include <memory>
#include <map>
#include <functional>


using LightCtor = std::function<std::unique_ptr<ILight>(const libconfig::Setting &s)>;

std::map<std::string, LightCtor> lightRegistry;
/*
LightFactory::LightFactory() {
  lightRegistry["Directional"] = [](const libconfig::Setting& s) {
    return std::make_unique<Directional>(s);
  };
  lightRegistry["Ambiant"] = [](const libconfig::Setting& s) {
    return std::make_unique<Ambiant>(s);
  };
}
*/
