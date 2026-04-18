#include "Default.hpp"
#include "Ray.hpp"

Default::Default([[maybe_unused]] const libconfig::Setting& s){
    this->type = "Default";
    this->color = {255, 255, 255};

    this->metallic = 0.0f;
    this->roughness = 0.5f;
    this->ior = 1.5f;
}

Default::Default(){
    this->type = "Default";
    this->color = {255, 255, 255};

    this->metallic = 0.0f;
    this->roughness = 0.5f;
    this->ior = 1.5f;
}

bool Default::scatter(const Ray& in, const Hit& hit, Vec3& attenuation, Ray& scattered) const 
{
    if (this->roughness > 0.2f && this->metallic < 0.5f)
        return false;

    Vec3 unitDir = normalize(in.dir);
    Vec3 reflected = normalize(reflect(unitDir, hit.normal));
    Vec3 offsetOrigin = hit.position + hit.normal * 0.001f;

    scattered = Ray(offsetOrigin, reflected);

    Vec3 F0 = Vec3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, this->color, this->metallic);
    
    attenuation = F0;

    return dot(reflected, hit.normal) > 0.0f;
}