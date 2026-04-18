#include "Glass.hpp"

Glass::Glass(const libconfig::Setting& s)
{
    this->type = "Glass";
    this->color = {255, 255, 255};
    this->metallic = 0.0f;
    this->roughness = 0.0f;
    this->ior = 1.5f;
    this->transmission = 1.0f;
}

Glass::Glass() 
{
    this->type = "Glass";
    this->color = {255, 255, 255};
    this->metallic = 0.0f;
    this->roughness = 0.0f;
    this->ior = 1.5f;
    this->transmission = 1.0f;
}

bool Glass::scatter(const Ray& inRay, const Hit& hit, Vec3& attenuation, Ray& scattered) const 
{
    return false;
}
