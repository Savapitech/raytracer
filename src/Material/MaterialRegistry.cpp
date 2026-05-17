
#include "MaterialRegistry.hpp"



namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

namespace material {
    using namespace space;
    using namespace shape;
    using namespace object;
    using namespace scene;

void MaterialRegistry::uploadMaterial(const std::string &materialName,
                                      const Material &material) {
  _materialRegistry[materialName] = material;
  Log::Logger::debug("Material uploaded to registry: " + materialName);
}

Material MaterialRegistry::getMaterial(const std::string &materialName) {
  auto it = _materialRegistry.find(materialName);

  if (it != _materialRegistry.end()) {
    return it->second;
  }
  Log::Logger::warning("Material not found in MTL: " + materialName);

  Material defaultMat;
  defaultMat.type = "default";
  defaultMat.color = {150, 0, 150};
  defaultMat.metallic = 0.0f;
  defaultMat.roughness = 0.5f;
  defaultMat.ior = 1.5f;
  defaultMat.transmission = 0.0f;
  defaultMat.textureType = TextureType::NONE;
  defaultMat.textureIndex = -1;

  return defaultMat;
}
} // namespace material
