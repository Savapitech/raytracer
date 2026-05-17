#include "Chrome.hpp"



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

Chrome::Chrome(const libconfig::Setting &) {
  this->type = "Chrome";
  this->color = {255, 255, 255};
  this->metallic = 1.0f;
  this->roughness = 0.05f;
  this->ior = 1.0f;
  this->textureType = TextureType::NONE;
}

Chrome::Chrome() {
  this->type = "Chrome";
  this->color = {255, 255, 255};
  this->metallic = 1.0f;
  this->roughness = 0.05f;
  this->ior = 1.0f;
  this->textureType = TextureType::NONE;
}
} // namespace material
