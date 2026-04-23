#include "Mirror.hpp"
#include "Ray.hpp"

Mirror::Mirror(const libconfig::Setting&)
{
    this->type = "Mirror";
    this->color = {255, 255, 255};
    this->metallic = 1.0f;
    this->roughness = 0.05f;
    this->ior = 1.0f;
}

bool Mirror::scatter(const Ray& in, const Hit& hit, Vec3& attenuation, Ray& scattered) const 
{
    if (this->roughness > 0.2f && this->metallic < 0.5f)
        return false;

    Vec3 unitDir = normalize(in.dir);
    Vec3 reflected = normalize(reflect(unitDir, hit.normal));

    Vec3 offsetOrigin = hit.position + hit.normal * 0.001f;

    scattered = Ray(offsetOrigin, reflected);
    attenuation = this->color;
    return dot(reflected, hit.normal) > 0.0f;
}