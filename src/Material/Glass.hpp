#pragma once

#include "IMaterials.hpp"

class Glass : public AMaterial  
{
    public:
        Glass(const libconfig::Setting& s);
        Glass();
        bool scatter(const Ray& inRay, const Hit& hit, Vec3& attenuation, Ray& scattered) const override;
};