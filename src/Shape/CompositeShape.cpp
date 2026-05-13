#include "CompositeShape.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include <algorithm>

CompositeShape::CompositeShape(const libconfig::Setting &s) {
    _type = "Composite";
    _pos = {0, 0, 0};
    _color = {255, 255, 255};

    if (s.exists("children")) {
        const libconfig::Setting& childrenList = s["children"];
        for (int i = 0; i < childrenList.getLength(); i++) {
            _children.push_back(getObjectFactory().shapeFactory.getShape(childrenList[i]));
        }
    }
}

bool CompositeShape::intersect(Ray &ray, Hit &hit) const noexcept {
    bool hitAnything = false;
    Hit tempHit;
    float closestSoFar = ray.maxHit;

    for (const auto& child : _children) {
        Ray tempRay = ray; 
        tempRay.maxHit = closestSoFar;
        if (child->intersect(tempRay, tempHit)) {
            hitAnything = true;
            closestSoFar = tempHit.t;
            hit = tempHit;
        }
    }
    return hitAnything;
}

AABB CompositeShape::getObjectAABB() const noexcept {
    if (_children.empty()) 
        return AABB(Vec3(0,0,0), Vec3(0,0,0));

    AABB result = _children[0]->getObjectAABB();

    for (size_t i = 1; i < _children.size(); i++) {
        AABB childBox = _children[i]->getObjectAABB();
        result.min.x = std::min(result.min.x, childBox.min.x);
        result.min.y = std::min(result.min.y, childBox.min.y);
        result.min.z = std::min(result.min.z, childBox.min.z);
        result.max.x = std::max(result.max.x, childBox.max.x);
        result.max.y = std::max(result.max.y, childBox.max.y);
        result.max.z = std::max(result.max.z, childBox.max.z);
    }
    return result;
}

Vec3 CompositeShape::getCentroid() const noexcept {
    AABB box = getObjectAABB();
    return (box.min + box.max) * 0.5f;
}

Vec2 CompositeShape::getUv(Vec3 &) const noexcept {
    return {0, 0};
}