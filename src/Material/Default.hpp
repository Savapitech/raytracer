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

class Default final : public Material {
public:
  Default(const libconfig::Setting &s);
  Default();
};

} // namespace material
