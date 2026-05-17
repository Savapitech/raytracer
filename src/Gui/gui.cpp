#include "Sfml.hpp"
#include <SFML/Graphics.hpp>
#include <atomic>
#include <chrono>
#include <cmath>

namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

using namespace graphical;


void gui() {
  Sfml s((std::uint16_t)1920, (std::uint16_t)1080);

  sf::Texture t(sf::Vector2u(WIDTH, HEIGHT));
  sf::Sprite sp(t);

  if (!t.loadFromFile("Asset/gui.png")) {
    exit(0);
  }
  sp.setTexture(t);
  s.addSprite(sp);
  while (s.isOpen()) {
    s.display();
    s.handleGui();
  }
}