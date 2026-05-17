#pragma once

#include "IMaterials.hpp"
#include "map"



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

class MaterialRegistry {
public:
  MaterialRegistry() = default;
  ~MaterialRegistry() = default;

  void uploadMaterial(const std::string &materialName,
                      const Material &material);
  Material getMaterial(const std::string &materialName);

private:
  std::map<std::string, Material> _materialRegistry;
};

} // namespace material
