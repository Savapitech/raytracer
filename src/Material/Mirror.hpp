#pragma once

#include "IMaterials.hpp"

class Mirror : public AMaterial  
{
    public:
        Mirror(const libconfig::Setting& s);
        Mirror() = default;
        bool scatter(const Ray& inRay, const Hit& hit, Vec3& attenuation, Ray& scattered) const override;

        float reflectlvl;
};