#include "IMaterials.hpp"

#include "Scene.hpp"



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

Perso::Perso(const libconfig::Setting &s) {
  this->type = "Perso";
  if (s.exists("color"))
    this->color = scene::readVec3(s["color"]);
  if (s.exists("colorChess"))
    this->colorChess = scene::readVec3(s["colorChess"]);
  if (s.exists("mettalic"))
    this->metallic = (float)(s["mettalic"]);
  else
    this->metallic = 0.0f;
  if (s.exists("roughness"))
    this->roughness = (float)(s["roughness"]);
  else
    this->roughness = 0.5f;
  if (s.exists("ior"))
    this->ior = (float)(s["ior"]);
  else
    this->ior = 1.5f;
  if (s.exists("texture")) {
    std::string texture = (std::string)s["texture"];

    if (texture == "chessboard")
      this->textureType = TextureType::CHESSBOARD;
    else {
      Log::Logger::debug("Load Texture:" + texture);
      std::cout << Material::textureManager.uploadTexture(texture) << std::endl;
      this->textureType = TextureType::LOAD_IMAGE;
    }
  } else {
    this->textureType = TextureType::NONE;
  }
  if (s.exists("transmission"))
    this->transmission = (float)(s["transmission"]);
  else
    this->transmission = 0.0f;
}
} // namespace material
