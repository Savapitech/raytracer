#pragma once

#include <memory>
#include <libconfig.h++>

#include "AABB.hpp"
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
        virtual Vec2 getUv(Vec3 &hitPos) const = 0;
        virtual const std::string &getType() const = 0;
        virtual void setType(std::string type) = 0;
        virtual const Vec3 getPos() const = 0;
        virtual void setPos(Vec3 pos) = 0;
        virtual const Vec3 getColor() const = 0;
        virtual void setColor(Vec3 color) = 0;
};

class AShape : public IShape
{
    public:
        void ShowShape(void){Log::Logger::debug("Object Shape:" + _type);}
        const std::string &getType() const override{return _type;}
        void setType(std::string type) override {_type = type;}
        const Vec3 getPos() const override {return _pos;}
        void setPos(Vec3 pos) override {pos = _pos;}
        const Vec3 getColor() const override {return _color;}
        void setColor(Vec3 color)  override {color = _color;}
    protected:
        std::string _type;
        Vec3 _pos;
        Vec3 _color;
        
};

class ShapeFactory
{
    public:
        ShapeFactory();
        std::unique_ptr<IShape> getShape(const libconfig::Setting &s);
};