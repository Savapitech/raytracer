#include "Glass.hpp"



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

Glass::Glass(const libconfig::Setting &) {
  this->type = "Glass";
  this->color = {255, 255, 255};
  this->metallic = 0.0f;
  this->roughness = 0.0f;
  this->ior = 1.5f;
  this->transmission = 1.0f;
  this->textureType = TextureType::NONE;
}

Glass::Glass() {
  this->type = "Glass";
  this->color = {255, 255, 255};
  this->metallic = 0.0f;
  this->roughness = 0.0f;
  this->ior = 1.5f;
  this->transmission = 1.0f;
  this->textureType = TextureType::NONE;
}

} // namespace material
