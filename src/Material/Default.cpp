#include "Default.hpp"
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

Default::Default([[maybe_unused]] const libconfig::Setting &s) {
  this->type = "Default";
  this->color = {255, 255, 255};

  this->metallic = 0.0f;
  this->roughness = 0.5f;
  this->ior = 1.5f;
  this->textureType = TextureType::NONE;
}

Default::Default() {
  this->type = "Default";
  this->color = {255, 255, 255};

  this->metallic = 0.0f;
  this->roughness = 0.5f;
  this->ior = 1.5f;
  this->textureType = TextureType::NONE;
}

} // namespace material
