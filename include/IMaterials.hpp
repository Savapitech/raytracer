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

        float reflectivity;
        float shininess;
        Vec3 Ks;
};

class Mirror : public AMaterial  
{
    public:
        Mirror(const libconfig::Setting& s);
        Mirror() = default;
        bool scatter(const Ray& inRay, const Hit& hit, Vec3& attenuation, Ray& scattered) const override;

        float reflectlvl;
};

class Chrome : public Mirror
{
    public:
        Chrome(const libconfig::Setting& s)
        {
            this->type = "Chrome";
            this->reflectlvl = (float)s["reflect"];
            this->color = {255, 255, 255};
            this->isFong = false;
            this->reflectivity = 0.7f;
            this->shininess = 150.0f;
            this->Ks = Vec3(0.9f, 0.9f, 0.9f);
        }
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
