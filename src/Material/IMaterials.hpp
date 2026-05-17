#pragma once

#include <libconfig.h++>
#include <memory>

#include "TextureManager.hpp"
#include "logger.hpp"
#include "math3d.hpp"



namespace object {}
namespace shape {}
namespace space {}
class Ray;
class Hit;

namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

class Ray;
class Hit;

namespace material {
    using namespace space;
    using namespace shape;
    using namespace object;
    using namespace scene;

class Material {
public:
  void ShowMaterial(void) { Log::Logger::debug("Object Material:" + type); }
  static TextureManager textureManager;
  Material() {}
  Material(const libconfig::Setting &s);

  int textureIndex = -1;
  std::string type;
  Vec3 color;
  Vec3 colorChess;

  float metallic;
  float roughness;
  float ior;
  float transmission = 0.0f;

  TextureType textureType;
};

class Perso : public Material {
public:
  Perso(const libconfig::Setting &s);
  void ShowMaterial(void) { Log::Logger::debug("Object Material:" + type); }
  // static TextureManager textureManager;
};

class MaterialFactory {
public:
  MaterialFactory();
  // TextureManager textureManager;
  std::unique_ptr<Material> getMaterial(const libconfig::Setting &s);
};

} // namespace material
