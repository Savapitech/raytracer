#pragma once

#include "math3d.hpp"

class ILight
{
public:
    virtual ~ILight() = default;

    virtual Vec3 getCenter() const = 0;
    virtual Vec3 getColor() const = 0;
    virtual Vec3 getSamplePosition() const = 0;
    virtual Vec3 getRadiance(const Vec3& hitPoint) const = 0;
    virtual float getSize() const = 0;
    virtual bool castsShadows() const = 0; 
};

class ALight : public ILight
{
public:
    Vec3 getColor() const override { return _color; }
    float getSize() const override { return _size; }

protected:
    Vec3 _color;
    float _size;
    Vec3 _position;
};


class LightFactory
{
    public:
        LightFactory();
        std::unique_ptr<ILight> getLight(const libconfig::Setting &s);
};
