#pragma once

#include <memory>
#include <libconfig.h++>

#include "logger.hpp"
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

        int textureIndex = -1;
        std::string type;
        Vec3 color;

        float metallic;
        float roughness;
        float ior;
        float transmission = 0.0f;
};

class MaterialFactory{
    public:
        MaterialFactory();
        TextureManager textureManager;
        std::unique_ptr<AMaterial> getMaterial(const libconfig::Setting &s);
};
