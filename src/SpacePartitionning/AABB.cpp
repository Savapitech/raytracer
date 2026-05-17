#include "AABB.hpp"
#include "Ray.hpp"



namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

namespace space {
    using namespace object;
    using namespace shape;
    using namespace material;

void AABB::normalize() {
  if (min.x > max.x)
    std::swap(min.x, max.x);
  if (min.y > max.y)
    std::swap(min.y, max.y);
  if (min.z > max.z)
    std::swap(min.z, max.z);
}

bool AABB::intersect(const Ray &r) const {
  float tMin = r.minHit;
  float tMax = r.maxHit;

  for (int i = 0; i < 3; i++) {
    float t0 = (min[i] - r.origin[i]) * r.invDir[i];
    float t1 = (max[i] - r.origin[i]) * r.invDir[i];

    float tNear = std::min(t0, t1);
    float tFar = std::max(t0, t1);

    tMin = std::max(tMin, tNear);
    tMax = std::min(tMax, tFar);

    if (tMax <= tMin)
      return false;
  }

  return true;
}
} // namespace space
