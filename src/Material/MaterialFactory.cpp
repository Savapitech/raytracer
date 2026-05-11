#include <functional>
#include <libconfig.h++>
#include <map>
#include <memory>

#include "Chrome.hpp"
#include "Default.hpp"
#include "Glass.hpp"
#include "Mirror.hpp"

#include "Scene.hpp"

#define MATERIAL "material"
#define TEXTURE "texture"

using MaterialCtor =
    std::function<std::unique_ptr<Material>(const libconfig::Setting &s)>;

std::map<std::string, MaterialCtor> materialRegistry;

MaterialFactory::MaterialFactory() {
  materialRegistry["mirror"] = [](const libconfig::Setting &s) {
    return std::make_unique<Mirror>(s);
  };
  materialRegistry["chrome"] = [](const libconfig::Setting &s) {
    return std::make_unique<Chrome>(s);
  };
  materialRegistry["glass"] = [](const libconfig::Setting &s) {
    return std::make_unique<Glass>(s);
  };
  materialRegistry["default"] = [](const libconfig::Setting &s) {
    return std::make_unique<Default>(s);
  };
  materialRegistry["perso"] = [](const libconfig::Setting &s) {
    return std::make_unique<Perso>(s);
  };
}

TextureManager Material::textureManager;

std::unique_ptr<Material>
MaterialFactory::getMaterial(const libconfig::Setting &s) {

  int textureIndex = -1;
  std::string type;
  std::unique_ptr<Material> Material;

  if (s.exists(MATERIAL)) {
    const libconfig::Setting &s1 = s[MATERIAL];
    Log::Logger::debug("Material FOUND");
    if (!s1.exists("type"))
      throw std::invalid_argument("Need type to implement Type");
    Log::Logger::debug("Type FOUND");
    type = (std::string)s1["type"];
    Log::Logger::debug("Type load");
  } else
    type = "default";

  auto fcntMaterial = materialRegistry.at(type);

  if (fcntMaterial == nullptr)
    throw std::invalid_argument("Type: " + type + " didn't exist.");

  if (type == "default")
    Material = fcntMaterial(s);
  else
    Material = fcntMaterial(s[MATERIAL]);
  if (Material->textureType == TextureType::LOAD_IMAGE)
    textureIndex = Material::textureManager.uploadTexture(s[MATERIAL][TEXTURE]);
  Material->textureIndex = textureIndex;
  Log::Logger::debug("IndexTexture: " + std::to_string(textureIndex));
  return Material;
}