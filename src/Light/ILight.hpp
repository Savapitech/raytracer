#pragma once

#include "math3d.hpp"
#include <libconfig.h++>



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

class ILight {
public:
  virtual ~ILight() = default;

  virtual Vec3 getCenter() const = 0;
  virtual Vec3 getColor() const = 0;
  virtual Vec3 getSamplePosition() const = 0;
  virtual Vec3 getRadiance(const Vec3 &hitPoint) const = 0;
  virtual float getSize() const = 0;
  virtual bool castsShadows() const = 0;
};

class ALight : public ILight {
public:
  Vec3 getColor() const override { return _color; }
  float getSize() const override { return _size; }

protected:
  Vec3 _color;
  float _size;
  Vec3 _position;
};

class LightFactory {
public:
  LightFactory();
  std::unique_ptr<ILight> getLight(const libconfig::Setting &s);
};

} // namespace light
