#include "Mandelbulb.hpp"

Mandelbulb::Mandelbulb(const libconfig::Setting &s) {
  _type = "Mandelbulb";
  _power = (float)s["power"];
  _pos = scene::readVec3(s["pos"]);
  _color = scene::readVec3(s["color"]);
  _iterations = (int)s["iterations"];
  _juliaMode = s.exists("juliaMode") ? (bool)s["juliaMode"] : false;
  _juliaConstant = s.exists("juliaConstant")
                       ? scene::readVec3(s["juliaConstant"])
                       : Vec3(0, 0, 0);
}

AABB Mandelbulb::getObjectAABB() const {
  float size = 1.5f;

  return {{_pos.x - size, _pos.y - size, _pos.z - size},
          {_pos.x + size, _pos.y + size, _pos.z + size}};
}

Vec3 Mandelbulb::getCentroid() const { return _pos; }

float Mandelbulb::evaluateSDF(const Vec3 &p) const {
  Vec3 z = p;
  float dr = 1.0f;
  float r = 0.0f;
  for (int i = 0; i < _iterations; i++) {
    r = norm(z);
    if (r > 2.0f)
      break;
    float theta = std::acos(z.y / r);
    float phi = std::atan2(z.z, z.x);
    if (_juliaMode) {
      dr = std::pow(r, _power - 1.0f) * _power * dr;
    } else {
      dr = std::pow(r, _power - 1.0f) * _power * dr + 1.0f;
    }
    float zr = std::pow(r, _power);
    theta = theta * _power;
    phi = phi * _power;
    Vec3 vec(std::sin(theta) * std::cos(phi), std::cos(theta),
             std::sin(theta) * std::sin(phi));
    z = vec * zr;
    if (_juliaMode) {
      z += _juliaConstant;
    } else
      z += p;
  }
  return 0.5 * std::log(r) * r / std::max(dr, 0.00001f);
}
