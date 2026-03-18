#include "RayTracer.hpp"
#include <cmath>
#include <algorithm>


Vec3 speculaire(const Vec3& V, const Vec3& N, const Vec3& L, float alpha) noexcept
{
    Vec3 lightColor(255, 255, 255);
    Vec3 R = normalize( N * (2.0f * dot(N, L)) - L );
    float specAngle = std::max(dot(R, V), 0.0f);
    float spec = std::pow(specAngle, alpha);
    return lightColor * spec;
}

bool Render::ShadowRay(const Vec3 &light, Vec3 &P, Vec3 &L, Vec3 &N, int index) noexcept
{
    Hit shadowHit;
    float espilon = 1e-4 * ((float(1.0f) > P.length()) ? 1.0f : P.length()) ;
    Ray shadowRay(P + N * espilon, L); 
    float distToLight = norm(light - P);

    if (this->bvh.intersect(shadowRay, shadowHit) == true) {
        if (shadowHit.ObjectIdx == index)
            return false;
        if (shadowHit.t > 0.0f && shadowHit.t < distToLight)
            return true;
    }
    return false;
}

/*Il faudrait que je parcours une list de light plus tard la variable Light est provisoire*/
Vec3 Render::AppliedFong(Ray &ray, Hit &minHit, const Vec3& albedo) noexcept
{
    const auto& material = this->scene.getObjects()[minHit.ObjectIdx]->material;

    Vec3 finalColor = Vec3(255, 0, 255) * 0.1f;

    Vec3 P = minHit.position;
    Vec3 N = minHit.normal;

    if (dot(N, ray.dir) > 0.0f) {
        N = N * -1.0f;
    }

    Vec3 V = normalize(-ray.dir);
    

    std::vector<Vec3> lights = { Vec3(5, 200, -50) };
    lights.push_back(ray.origin);

    for (const auto& lightPos : lights) 
    {
        Vec3 L = normalize(lightPos - P);
        if (ShadowRay(lightPos, P, L, N, minHit.ObjectIdx) == true)
            continue; 

        float diff = std::max(dot(N, L), 0.0f);
        Vec3 diffuseColor = albedo * diff;
        Vec3 spec = speculaire(V, N, L, material->shininess);        
        Vec3 specularColor = spec * material->Ks; 
        finalColor += diffuseColor + specularColor;
    }
    finalColor.x = std::clamp(finalColor.x, 0.0f, 255.0f);
    finalColor.y = std::clamp(finalColor.y, 0.0f, 255.0f);
    finalColor.z = std::clamp(finalColor.z, 0.0f, 255.0f);

    return finalColor;
}

sf::Color Render::shade(Ray &ray, Hit &hit, int depth) noexcept
{

    if (depth <= 0)
        return sf::Color(255, 255, 255, 255);

    const auto& object = this->scene.getObjects()[hit.ObjectIdx];
    const auto& material = object->material;

    Vec3 albedo = object->shape->getColor();
    
    if (material->textureIndex > -1) {
        Vec2 uv = object->shape->getUv(hit.position);
        albedo = material->textureManager.getTexturePix(material->textureIndex, uv);
    }

    Vec3 finalColor = AppliedFong(ray, hit, albedo); 

    if (material->reflectivity > 0.0f) {
        Vec3 reflectDir = normalize(reflect(normalize(ray.dir), hit.normal));
        Ray reflectedRay(hit.position + hit.normal * 0.001f, reflectDir);
        Hit reflectHit;
        Vec3 bounceColor(255, 255, 255);

        if (this->bvh.intersect(reflectedRay, reflectHit) == true) {
            sf::Color childColorSf = this->shade(reflectedRay, reflectHit, depth - 1);
            bounceColor = Vec3(childColorSf.r, childColorSf.g, childColorSf.b);
        }
        else {
            bounceColor = Vec3(255.0f, 255.0f, 255.0f);
        }
        finalColor = lerp(finalColor, bounceColor, material->reflectivity);
    }
    return sf::Color(finalColor.x, finalColor.y, finalColor.z, 255);
}
