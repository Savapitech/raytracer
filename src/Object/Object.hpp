#pragma once
#include <libconfig.h++>
#include <memory>

#include "IMaterials.hpp"
#include "IShape.hpp"

class Ray;
class Hit;


namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

namespace object {
    using namespace space;
    using namespace shape;
    using namespace material;

class ObjectFactory {
public:
  ShapeFactory shapeFactory;
  MaterialFactory materialFactory;
};

class Object {
public:
  Object(const libconfig::Setting &s);
  Object(std::unique_ptr<IShape> shape, std::unique_ptr<Material> material);

  const std::unique_ptr<IShape> &getShape() const { return _shape; }
  const std::unique_ptr<Material> &getMaterial() const { return _material; }
  const AABB &getAABB() const { return _aabb; }
  Vec3 getCentroid() const { return _centroid; }

private:
  AABB _aabb;
  std::unique_ptr<IShape> _shape;
  std::unique_ptr<Material> _material;
  Vec3 _centroid;
};

inline ObjectFactory &getObjectFactory() {
  static ObjectFactory instance;
  return instance;
}
} // namespace object
