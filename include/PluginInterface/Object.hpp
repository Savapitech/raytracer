#pragma once
#include <iostream>
#include <memory>
#include <libconfig.h++>

#include "logger.hpp"
#include "math3d.hpp"

class Ray;
class Hit;

class IShape
{
    public:
        virtual ~IShape() = default;
        virtual bool intersect(Ray &ray, Hit &hit) const = 0;
        virtual AABB getObjectAABB() const = 0;
        virtual Vec3 getCentroid() const = 0;
        //virtual const Vec3& getPos(void) const = 0;
};

class AShape : public IShape
{
    public:
        void ShowShape(void){Log::Logger::debug("Object Shape:" + type);}
        std::string type;
        Vec3 pos;
        Vec3 color;
};

class Sphere final : public AShape  
{
    public:
        float radius;
        bool intersect(Ray &ray, Hit &hit) const override;
        AABB getObjectAABB() const override;
        Vec3 getCentroid() const override;
        Sphere(const libconfig::Setting& s);
};

class ShapeFactory{
    public:
        ShapeFactory();
        std::unique_ptr<AShape> GetShape(const libconfig::Setting &s);
};

class ObjectFactory{
        public:
            /*std::unique_ptr<IMaterial> GetShape(const Setting &s);*/
            ShapeFactory SFactory;
};

class Object
{
    public:
        Object(const libconfig::Setting &s, bool materialExist);
        AABB aabb;
        std::unique_ptr<AShape> shape;
        Vec3 getCentroid() const {
            return centroid;
}
    private:
        Vec3 centroid;
        static ObjectFactory OFactory;
};
