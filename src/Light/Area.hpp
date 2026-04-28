#pragma once

#include "ILight.hpp"
#include "Scene.hpp"

class AreaLight : public ALight
{
public:
    AreaLight(Vec3 pos, Vec3 col, float s) {
        this->_position = pos;
        this->_color = col;
        this->_size = s;
    }

    AreaLight(const libconfig::Setting& s) {
      this->_position = scene::readVec3(s["pos"]);
      this->_color = scene::readVec3(s["color"]);
      this->_size = (float)s["size"];
    }

    Vec3 getCenter() const override { return _position; }

    Vec3 getSamplePosition() const override {
        float halfSize = _size / 2.0f;
        float jitterX = fastRandomFloat(-halfSize, halfSize);
        float jitterZ = fastRandomFloat(-halfSize, halfSize);
        return _position + Vec3(jitterX, 0.0f, jitterZ);
    }

    Vec3 getRadiance(const Vec3& hitPoint) const override {
        return _color; 
    }

    bool castsShadows() const override { return true; }
};
