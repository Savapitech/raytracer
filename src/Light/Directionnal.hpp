#pragma once

#include "ILight.hpp"

class DirectionnalLight : public ALight {
public:
  DirectionnalLight(Vec3 pos, Vec3 col, float s) {
    this->_position = normalize(pos);
    this->_color = col;
    this->_size = s;
  }

  DirectionnalLight(const libconfig::Setting &s) {
    this->_position = normalize(scene::readVec3(s["pos"]));
    this->_color = scene::readVec3(s["color"]);
    this->_size = (float)s["size"];
  }

  Vec3 getCenter() const override { return _position; }

  Vec3 getSamplePosition() const override { return _position * 10000.0f; }

  Vec3 getRadiance(const Vec3 &hitPoint) const override { return _color; }

  bool castsShadows() const override { return true; }
};
