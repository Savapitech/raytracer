#include "RayTracer.hpp"


Mirror::Mirror(const libconfig::Setting& s){
    this->type = "Mirror";
    this->reflectlvl = (float)s["reflect"];
    this->color = {255, 255, 255};
    this->isFong = false;
    this->reflectivity = 1.0f;
    this->shininess = 200.0f;
    this->Ks = Vec3(1.0f, 1.0f, 1.0f);
}            


bool Mirror::scatter(const Ray& in, const Hit& hit, Vec3& attenuation, Ray& scattered) const 
{
    Vec3 unitDir = normalize(in.dir);
    Vec3 reflected = reflect(unitDir, hit.normal);
    
        reflected = normalize(reflected);
    Vec3 offsetOrigin = hit.position + hit.normal * 0.0001f;
    scattered = Ray(offsetOrigin, reflected);
    attenuation = Vec3(1.f, 1.0f, 1.0f);
    return dot(reflected, hit.normal) > 0.0f;
}