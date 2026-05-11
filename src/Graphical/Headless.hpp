#pragma once
#include "IGraphical.hpp"

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
