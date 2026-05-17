#pragma once
#include "IGraphical.hpp"



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

class Headless : public IGraphical {
public:
  void display() override {}
  bool isOpen() override { return true; }
  void addSprite(sf::Sprite &) override {}
  void handleEvent() override {}
  void handleGui() override {}
  bool handleMovement(scene::Scene &) override { return false; }
  void save(const uint8_t *, uint32_t, uint32_t) override {}
  bool needsLiveUpdate() const override { return false; }
};

} // namespace graphical
