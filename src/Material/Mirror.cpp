#include "Mirror.hpp"
#include "Ray.hpp"



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

Mirror::Mirror(const libconfig::Setting &) {
  this->type = "Mirror";
  this->color = {255, 255, 255};
  this->metallic = 1.0f;
  this->roughness = 0.05f;
  this->ior = 1.0f;
  this->textureType = TextureType::NONE;
}

Mirror::Mirror() {
  this->type = "Mirror";
  this->color = {255, 255, 255};
  this->metallic = 1.0f;
  this->roughness = 0.05f;
  this->ior = 1.0f;
  this->textureType = TextureType::NONE;
}
} // namespace material
