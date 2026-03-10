#pragma once
#include <iostream>
#include <memory>
#include <libconfig.h++>

#include "IShape.hpp"
#include "IMaterials.hpp"
#include "logger.hpp"

class Ray;
class Hit;

class ObjectFactory{
        public:
            ShapeFactory SFactory;
            MaterialFactory MFactory;
};

class Object
{
    public:
        Object(const libconfig::Setting &s);
        Object(std::unique_ptr<AShape> shape, std::unique_ptr<AMaterial> material);
        AABB aabb;
        std::unique_ptr<IShape> shape;
        std::unique_ptr<AMaterial> material;
        Vec3 getCentroid() const {
            return centroid;
        }
    private:
        Vec3 centroid;
};
