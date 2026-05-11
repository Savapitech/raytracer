#include "Sfml.hpp"
#include <SFML/Graphics.hpp>
#include <atomic>
#include <chrono>
#include <cmath>

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