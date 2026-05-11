#include "Mandelbox.hpp"

Mandelbox::Mandelbox(const libconfig::Setting &s) {
  _type = "Mandelbox";
  _scale = s.exists("scale") ? (float)s["scale"] : -1.5f;
  _pos = scene::readVec3(s["pos"]);
  _color = scene::readVec3(s["color"]);
  _iterations = (int)s["iterations"];
  _juliaMode = s.exists("juliaMode") ? (bool)s["juliaMode"] : false;
  _juliaConstant = s.exists("juliaConstant")
                       ? scene::readVec3(s["juliaConstant"])
                       : Vec3(0, 0, 0);
}

AABB Mandelbox::getObjectAABB() const {
  float size = 6.0f;

  return {{_pos.x - size, _pos.y - size, _pos.z - size},
          {_pos.x + size, _pos.y + size, _pos.z + size}};
}

Vec3 Mandelbox::getCentroid() const { return _pos; }

float Mandelbox::evaluateSDF(const Vec3 &p) const {
  Vec3 z = p;
  float dr = 1.0f;
  float scale = _scale;

  for (int i = 0; i < _iterations; i++) {
    z.x = std::clamp(z.x, -1.0f, 1.0f) * 2.0f - z.x;
    z.y = std::clamp(z.y, -1.0f, 1.0f) * 2.0f - z.y;
    z.z = std::clamp(z.z, -1.0f, 1.0f) * 2.0f - z.z;
    float r2 = dot(z, z);
    if (r2 < 0.25f) {
      z = z * 4.0f;
      dr = dr * 4.0f;
    } else if (r2 < 1.0f) {
      float t = 1.0f / r2;
      z = z * t;
      dr = dr * t;
    }
    z = z * scale;
    if (_juliaMode) {
      z += _juliaConstant;
      dr = dr * std::abs(scale);
    } else {
      z += p;
      dr = dr * std::abs(scale) + 1.0f;
    }
  }
  return norm(z) / std::abs(dr);
}
