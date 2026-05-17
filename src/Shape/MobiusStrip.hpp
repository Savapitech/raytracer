#pragma once
#include "AFractals.hpp"
#include <algorithm>
#include <cmath>



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

class MobiusStrip : public AFractals {
public:
  MobiusStrip(const libconfig::Setting &s);
  Vec3 getCentroid() const override;
  float evaluateSDF(const Vec3 &p) const override;
  AABB getObjectAABB() const override;

private:
  float _stripRadius;
  float _thickness;
  float _width;
};

} // namespace shape
