#pragma once
#include "AFractals.hpp"
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

class Mandelbulb : public AFractals {
public:
  Mandelbulb(const libconfig::Setting &s);
  float evaluateSDF(const Vec3 &p) const override;
  AABB getObjectAABB() const override;
  Vec3 getCentroid() const override;

private:
  float _power;
  int _iterations;
  bool _juliaMode;
  Vec3 _juliaConstant;
};
} // namespace shape
