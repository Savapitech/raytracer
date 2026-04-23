#pragma once

#include "math3d.hpp"

//class LightFactory
//{
//    public:
//        LightFactory();
//        std::unique_ptr<ILight> getLight(const libconfig::Setting &s);
//};

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
    Vec3 getColor() const override { return color; }
    float getSize() const override { return size; }
    
protected:
    Vec3 color;
    float size;
    Vec3 position;
};

class AreaLight : public ALight
{
public:
    AreaLight(Vec3 pos, Vec3 col, float s) {
        this->position = pos;
        this->color = col;
        this->size = s;
    }

    Vec3 getCenter() const override { return position; }

    Vec3 getSamplePosition() const override {
        float halfSize = size / 2.0f;
        float jitterX = fastRandomFloat(-halfSize, halfSize);
        float jitterZ = fastRandomFloat(-halfSize, halfSize);
        return position + Vec3(jitterX, 0.0f, jitterZ);
    }

    Vec3 getRadiance(__attribute_maybe_unused__ const Vec3& hitPoint) const override {
        return color; 
    }

    bool castsShadows() const override { return true; }
};

class AmbiantLight : public ALight
{
public:
    AmbiantLight(Vec3 col) {
        this->color = col;
        this->size = 0.0f;
    }

    Vec3 getCenter() const override { return Vec3(0,0,0); }
    
    Vec3 getSamplePosition() const override { return Vec3(0,0,0); }
    Vec3 getRadiance(const Vec3&) const override { return color; }
    bool castsShadows() const override { return false; }
};