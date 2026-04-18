#pragma once

#include "IMaterials.hpp"

class Default final : public AMaterial
{
    public:
        Default(const libconfig::Setting& s);
        Default();
    
        bool scatter(const Ray& inRay, const Hit& hit, Vec3& attenuation, Ray& scattered) const override;
};
