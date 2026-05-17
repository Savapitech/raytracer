#include "Ambiant.hpp"
#include "Area.hpp"
#include "Directionnal.hpp"
#include "ILight.hpp"

#include <functional>
#include <libconfig.h++>
#include <map>
#include <memory>



namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

namespace light {
    using namespace space;
    using namespace shape;
    using namespace object;
    using namespace scene;

using LightCtor =
    std::function<std::unique_ptr<ILight>(const libconfig::Setting &s)>;

std::map<std::string, LightCtor> lightRegistry;

LightFactory::LightFactory() {
  lightRegistry["Directional"] = [](const libconfig::Setting &s) {
    return std::make_unique<DirectionnalLight>(s);
  };
  lightRegistry["Ambiant"] = [](const libconfig::Setting &s) {
    return std::make_unique<AmbiantLight>(s);
  };
  lightRegistry["Area"] = [](const libconfig::Setting &s) {
    return std::make_unique<AreaLight>(s);
  };
}

std::unique_ptr<ILight> LightFactory::getLight(const libconfig::Setting &s) {
  if (!s.exists("type"))
    throw std::invalid_argument("Need type to implement light");

  std::string type = s["type"];
  auto fcntShape = lightRegistry.at(type);
  if (fcntShape == nullptr) {
    throw std::invalid_argument("Type: " + type + " didn't exist.");
  }
  return fcntShape(s);
}

} // namespace light
