#include "MobiusStrip.hpp"



namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

namespace shape {
    using namespace space;
    using namespace object;
    using namespace material;
    using namespace scene;

MobiusStrip::MobiusStrip(const libconfig::Setting &s) {
  _type = "MobiusStrip";
  _pos = scene::readVec3(s["pos"]);
  _color = scene::readVec3(s["color"]);
  _stripRadius = s.exists("radius") ? (float)s["radius"] : 2.0f;
  _width = s.exists("width") ? (float)s["width"] : 0.5f;
  _thickness = s.exists("thickness") ? (float)s["thickness"] : 0.05f;
}

AABB MobiusStrip::getObjectAABB() const {
  float size = _stripRadius + _width + 0.1f;

  return {{_pos.x - size, _pos.y - size, _pos.z - size},
          {_pos.x + size, _pos.y + size, _pos.z + size}};
}

Vec3 MobiusStrip::getCentroid() const { return _pos; }

float MobiusStrip::evaluateSDF(const Vec3 &p) const {
  float theta = std::atan2(p.z, p.x);
  float radDist = std::sqrt((p.x * p.x) + (p.z * p.z));

  float qx = radDist - _stripRadius;
  float qy = p.y;
  float alpha = theta / 2.0f;
  float rx = qx * std::cos(alpha) - qy * std::sin(alpha);
  float ry = qx * std::sin(alpha) + qy * std::cos(alpha);
  float dx = std::abs(rx) - _width;
  float dy = std::abs(ry) - _thickness;

  return std::sqrt((std::max(dx, 0.0f) * std::max(dx, 0.0f)) +
                   (std::max(dy, 0.0f) * std::max(dy, 0.0f))) +
         std::min(std::max(dx, dy), 0.0f);
}

} // namespace shape
