#include "RayTracer.hpp"

bool Mirror::scatter(const Ray& in, const Hit& hit, Vec3& attenuation, Ray& scattered) const
{
    Vec3 unitDir = normalize(in.dir);
    Vec3 reflected(reflect);

    scattered = Ray(hit.position, reflected);
    attenuation = Vec3(1.0, 1.0, 1.0);
    return dot(scattered.dir, hit.normal) > 0;
}