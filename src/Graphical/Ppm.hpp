#pragma once

#include "IGraphical.hpp"
#include <string>

class Ppm : public IGraphical {
public:
  Ppm(const std::string &filename);

  void display(void) override {}
  bool isOpen(void) override { return !_saved; }
  void addSprite(sf::Sprite &) override {}

  void handleEvent(void) override {}
  void handleGui(void) override {}
  bool handleMovement(scene::Scene &) override { return false; }

  void save(const uint8_t *data, uint32_t width, uint32_t height) override;
  bool needsLiveUpdate(void) const override { return false; }

private:
  std::string _filename;
  bool _saved;
};
