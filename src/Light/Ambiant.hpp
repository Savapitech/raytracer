#pragma once

#include "ILight.hpp"
#include "Ray.hpp"

class AmbiantLight : public ALight {
public:
  AmbiantLight(Vec3 col) {
    this->_color = col;
    this->_size = 0.0f;
  }

  AmbiantLight(const libconfig::Setting &s) {
    this->_color = scene::readVec3(s["color"]);
    this->_size = (float)s["size"];
  }

  Vec3 getCenter() const override { return Vec3(0, 0, 0); }

  Vec3 getSamplePosition() const override { return Vec3(0, 0, 0); }
  Vec3 getRadiance(const Vec3 &hitPoint) const override { return _color; }
  bool castsShadows() const override { return false; }
};
