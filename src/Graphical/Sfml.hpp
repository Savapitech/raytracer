#pragma once

#include "IGraphical.hpp"

class Sfml : public IGraphical {
  public:
    Sfml(std::uint16_t width, std::uint16_t height);
    void display(void) override;
    bool isOpen(void) override;
    void addSprite(sf::Sprite &) override;

    void handleEvent(void) override;
    bool handleMovement(scene::Scene &) override;

  private:
    sf::RenderWindow window;
    std::vector<std::reference_wrapper<sf::Sprite>> spriteTab;
};
