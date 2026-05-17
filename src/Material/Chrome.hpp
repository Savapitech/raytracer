#pragma once

#include "Mirror.hpp"



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

class Chrome : public Mirror {
public:
  Chrome(const libconfig::Setting &s);
  Chrome();
};
} // namespace material
