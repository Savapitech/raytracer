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

class Sfml : public IGraphical {
public:
  Sfml(std::uint16_t width, std::uint16_t height);
  void display(void) override;
  bool isOpen(void) override;
  void addSprite(sf::Sprite &) override;

  void handleEvent(void) override;
  void handleGui(void) override;
  bool handleMovement(scene::Scene &) override;

  void save(const uint8_t *, uint32_t, uint32_t) override {}
  bool needsLiveUpdate(void) const override { return true; }

private:
  sf::RenderWindow _window;
  std::vector<std::reference_wrapper<sf::Sprite>> _spriteTab;
};

} // namespace graphical
