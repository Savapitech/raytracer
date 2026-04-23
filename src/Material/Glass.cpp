#include "Glass.hpp"

Glass::Glass(const libconfig::Setting&)
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

bool Glass::scatter(const Ray& , const Hit& , Vec3& , Ray& ) const 
{
    return false;
}
