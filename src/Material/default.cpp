#include "RayTracer.hpp"

Default::Default([[maybe_unused]] const libconfig::Setting& s){
    this->type = "Default";
    this->color = {255, 255, 255};
    this->isFong = true;
}            

bool Default::scatter(
    [[maybe_unused]] const Ray& inRay,[[maybe_unused]] const Hit& hit,
    Vec3& attenuation,[[maybe_unused]] Ray& scattered) const
{ 
    attenuation = Vec3(0.0f, 0.0f, 0.0f);
    return false;
}