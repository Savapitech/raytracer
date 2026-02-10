#include "RayTracer.hpp"

Vec3 speculaire(const Vec3& V, const Vec3& N, const Vec3& L, float alpha) noexcept
{
    Vec3 lightColor(255, 255, 255);
    Vec3 R = normalize( N * (2.0f * dot(N, L)) - L );
    float specAngle = std::max(dot(R, V), 0.0f);
    float spec = std::pow(specAngle, alpha);
    return lightColor * spec;
}

bool Render::ShadowRay(Vec3 &light, Hit &minHit, Vec3 &P, Vec3 &L) noexcept
{
    Ray shadowRay(P + minHit.normal * 0.001f, L);
    shadowRay.minHit = 0.001f;
    shadowRay.maxHit = norm(light - P) - 0.001f;

    if (this->bvh.intersect(shadowRay, minHit) == true)
        return true;
    return false;
}


/*Il faudrait que je parcours une list de light plus tard la variable Light est provisoire*/
Vec3 Render::AppliedFong(Ray &ray, Hit &minHit) noexcept
{
    Vec3 light(0, 20, -5);
    Vec3 colorShape = this->scene.getObjects()[minHit.ObjectIdx]->shape->color;

    Vec3 P = ray.origin + ray.dir * minHit.t;
    Vec3 L = normalize(light - P);
   
    Vec3 N = minHit.normal;

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

#define MAX_BOUNCE 10

sf::Color Render::shade(Ray &ray, Hit &hit) noexcept
{
    Vec3 color(0);
    Vec3 RelfectedIntensity(1, 1, 1);
    Hit tmpHit = hit;
    Vec3 reflectedColor;
    Vec3 textureColor = {1, 1, 1};

    if (this->scene.getObjects()[tmpHit.ObjectIdx]->material->textureIndex > -1) {
        Vec2 uv = this->scene.getObjects()[tmpHit.ObjectIdx]->shape->getUv(hit.position);
        textureColor = this->scene.getObjects()[tmpHit.ObjectIdx]->material->textureManager.getTexturePix(this->scene.getObjects()[tmpHit.ObjectIdx]->material->textureIndex, uv);
    }

    if (this->scene.getObjects()[hit.ObjectIdx]->material->isFong == true) {
        Vec3 fong = AppliedFong(ray, hit);
        fong = fong * textureColor;
        return sf::Color(fong.x, fong.y, fong.z, 255);
    }

    for (int i = 0; i != MAX_BOUNCE; i++) {
        Ray reflectedRay(tmpHit.position + tmpHit.normal * 0.001f, normalize(reflect(normalize(ray.dir), tmpHit.normal)));
        Ray scattered;
        if (this->bvh.intersect(reflectedRay, tmpHit) == false)
            return sf::Color(100,100,100,100);
        reflectedColor = AppliedFong(reflectedRay, tmpHit);
        if (this->scene.getObjects()[tmpHit.ObjectIdx]->material->scatter(reflectedRay, tmpHit, RelfectedIntensity, scattered) == false)
            break;
        reflectedColor *= RelfectedIntensity;
        if (RelfectedIntensity.length() < 0.01f)
            break;
    }
    if (RelfectedIntensity.x == 1 && RelfectedIntensity.y == 1 && RelfectedIntensity.z == 1)
        return sf::Color(100,100,100,100);
    return sf::Color(reflectedColor.x, reflectedColor.y, reflectedColor.z, 255);
}
