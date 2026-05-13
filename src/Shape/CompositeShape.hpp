#pragma once
#include "IShape.hpp"
#include <vector>
#include <memory>

class CompositeShape : public AShape {
public:
    CompositeShape(const libconfig::Setting &s);

    bool intersect(Ray &ray, Hit &hit) const noexcept override;
    AABB getObjectAABB() const noexcept override;
    Vec3 getCentroid() const noexcept override;
    Vec2 getUv(Vec3 &hitPos) const noexcept override;

private:
    std::vector<std::unique_ptr<IShape>> _children;
};