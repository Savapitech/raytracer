#include "SphereMarching.hpp"
#include "AFractals.hpp"
#include "Object.hpp"
#include "Ray.hpp"



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

SphereMarching::SphereMarching(const libconfig::Setting &s) {
  _type = "sphereMarching";
  _radius = (float)s["radius"];
  _pos = scene::readVec3(s["pos"]);
  _color = scene::readVec3(s["color"]);
}

AABB SphereMarching::getObjectAABB() const {
  return {{_pos.x - _radius, _pos.y - _radius, _pos.z - _radius},
          {_pos.x + _radius, _pos.y + _radius, _pos.z + _radius}};
}

Vec3 SphereMarching::getCentroid() const { return _pos; }

float SphereMarching::evaluateSDF(const Vec3 &p) const {
  return norm(p) - _radius;
}

} // namespace shape
