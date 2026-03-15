#include "RayTracer.hpp"

Default::Default([[maybe_unused]] const libconfig::Setting& s){
    this->type = "Default";
    this->color = {255, 255, 255};
    this->isFong = true;
    this->reflectivity = 0.0f;             
    this->shininess = 10.0f;
    this->Ks = Vec3(0.1f, 0.1f, 0.1f);
}

Default::Default(){
    this->type = "Default";
    this->color = {255, 255, 255};
    this->isFong = true;
    this->reflectivity = 0.0f;             
    this->shininess = 10.0f;
    this->Ks = Vec3(0.1f, 0.1f, 0.1f);
}            


bool Default::scatter(const Ray& in, const Hit& hit, Vec3& attenuation, Ray& scattered) const 
{
    if (this->reflectivity <= 0.0f)
        return false;

    Vec3 unitDir = normalize(in.dir);
    Vec3 reflected = normalize(reflect(unitDir, hit.normal));
    Vec3 offsetOrigin = hit.position + hit.normal * 0.001f;

    scattered = Ray(offsetOrigin, reflected);
    attenuation = this->Ks * this->reflectivity;

    return dot(reflected, hit.normal) > 0.0f;
}