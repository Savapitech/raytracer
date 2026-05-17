#pragma once
#include "AFractals.hpp"



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

class SphereMarching : public AFractals {

public:
  SphereMarching(const libconfig::Setting &s);
  float evaluateSDF(const Vec3 &p) const override;
  AABB getObjectAABB() const override;
  Vec3 getCentroid() const override;

private:
  float _radius;
};

} // namespace shape
