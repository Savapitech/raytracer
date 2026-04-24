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
};

class AMaterial : public IMaterial
{
    public:
        AMaterial(const libconfig::Setting& s);
        void ShowMaterial(void){Log::Logger::debug("Object Material:" + type);}
        static TextureManager textureManager;

        int textureIndex = -1;
        std::string type;
        Vec3 color;
        Vec3 colorChess;

        float metallic;
        float roughness;
        float ior;
        float transmission = 0.0f;

        TextureType textureType;
};

class MaterialFactory{
    public:
        MaterialFactory();
        TextureManager textureManager;
        std::unique_ptr<AMaterial> getMaterial(const libconfig::Setting &s);
};
