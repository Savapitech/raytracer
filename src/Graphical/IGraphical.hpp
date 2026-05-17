#pragma once

#include "Scene.hpp"



namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

namespace graphical {
    using namespace scene;
    using namespace object;
    using namespace space;
    using namespace shape;
    using namespace material;

class IGraphical {
public:
  virtual void display(void) = 0;
  virtual bool isOpen(void) = 0;
  virtual void addSprite(sf::Sprite &) = 0;

  virtual void handleEvent(void) = 0;
  virtual void handleGui(void) = 0;
  virtual bool handleMovement(scene::Scene &) = 0;

  virtual void save(const uint8_t *data, uint32_t width, uint32_t height) = 0;
  virtual bool needsLiveUpdate(void) const = 0;
};

} // namespace graphical
