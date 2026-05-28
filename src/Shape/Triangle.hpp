#pragma once
#include "IShape.hpp"

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

class Triangle : public AShape {
public:
  Triangle(const libconfig::Setting &s);
  Triangle() = default;
  Triangle(Vec3 x, Vec3 y, Vec3 z);
  
  // NOUVEAU: Constructeur avec UVs
  Triangle(Vec3 x, Vec3 y, Vec3 z, Vec2 uvX, Vec2 uvY, Vec2 uvZ);

  bool intersect(Ray &ray, Hit &hit) const noexcept override;
  AABB getObjectAABB() const noexcept override;
  Vec3 getCentroid() const noexcept override;
  Vec2 getUv(Vec3 &hitPos) const noexcept override;

  Vec3 x;
  Vec3 y;
  Vec3 z;

  Vec2 uvX = {0.0f, 0.0f};
  Vec2 uvY = {0.0f, 0.0f};
  Vec2 uvZ = {0.0f, 0.0f};
};

} // namespace shape