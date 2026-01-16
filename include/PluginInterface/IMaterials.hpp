#pragma once
#include <iostream>
#include <memory>
#include <libconfig.h++>

#include "math3d.hpp"
#include "TextureManager.hpp"

class Ray;
class Hit;

class IMaterial
{
    public:
        virtual ~IMaterial() = default;
        virtual bool scatter(const Ray& inRay, const Hit& hit, Vec3& attenuation, Ray& scattered) const = 0;
};

class AMaterial : public IMaterial
{
    public:
        void ShowMaterial(void){Log::Logger::debug("Object Material:" + type);}
        static TextureManager textureManager;

        std::string type;
        bool isFong = true;
        Vec3 color;
        int textureIndex;
};

class Mirror final : public AMaterial  
{
    public:
        Mirror(const libconfig::Setting& s);
        bool scatter(const Ray& inRay, const Hit& hit, Vec3& attenuation, Ray& scattered) const override;

        float reflectlvl;
};

class Default final : public AMaterial  
{
    public:
        Default(const libconfig::Setting& s);
        Default();
        bool scatter(const Ray& inRay, const Hit& hit, Vec3& attenuation, Ray& scattered) const override;
};

class MaterialFactory{
    public:
        MaterialFactory();
        TextureManager textureManager;
        std::unique_ptr<AMaterial> GetMaterial(const libconfig::Setting &s);
};