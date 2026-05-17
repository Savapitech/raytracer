#pragma once

#include "IMaterials.hpp"



namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

namespace material {
    using namespace space;
    using namespace shape;
    using namespace object;
    using namespace scene;

class Mirror : public Material {
public:
  Mirror(const libconfig::Setting &s);
  Mirror();

  float reflectlvl;
};
} // namespace material
