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

    void save(const uint8_t *, uint32_t, uint32_t) override {}
    bool needsLiveUpdate(void) const override { return true; }

  private:
    sf::RenderWindow _window;
    std::vector<std::reference_wrapper<sf::Sprite>> _spriteTab;
};
