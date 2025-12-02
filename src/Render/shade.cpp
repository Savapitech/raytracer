#include "RayTracer.hpp"

Vec3 speculaire(const Vec3& V, const Vec3& N, const Vec3& L, float alpha) noexcept
{
    Vec3 lightColor(255, 255, 255);
    Vec3 R = normalize( N * (2.0f * dot(N, L)) - L );
    float specAngle = std::max(dot(R, V), 0.0f);
    float spec = std::pow(specAngle, alpha);
    return lightColor * spec;
}


/*Il faudrait que je parcours une list de light plus tard la variable Light est provisoire*/
Vec3 Render::AppliedFong(Ray &ray, Hit &minHit) noexcept
{
    Vec3 light(10, 0, 10);
    Vec3 colorShape = minHit.object->shape->color;

    Vec3 P = ray.origin + ray.dir * minHit.t;
    Vec3 L = normalize(light - P);
    Vec3 N = normalize(P - minHit.object->shape->pos);

    if (ShadowRay(light, minHit, P, L) == true)
        return Vec3(0, 0, 0);
    
    float diff = std::max(dot(N, L), 0.0f);
    Vec3 spec = speculaire(normalize(-ray.dir), N, L, 50.f);
    Vec3 finalColor = spec + (colorShape * diff);

    finalColor.x = std::clamp(finalColor.x, 0.0f, 255.0f);
    finalColor.y = std::clamp(finalColor.y, 0.0f, 255.0f);
    finalColor.z = std::clamp(finalColor.z, 0.0f, 255.0f);

    return finalColor;
}

sf::Color Render::shade(Ray &ray, Hit &minHit) noexcept
{
    Vec3 finalColor = AppliedFong(ray, minHit);
    return sf::Color(finalColor.x, finalColor.y, finalColor.z, 255);
}