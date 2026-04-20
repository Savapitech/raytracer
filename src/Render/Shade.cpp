#include "Ray.hpp"
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <array>

Vec3 computeSpecular(const Vec3& viewDir, const Vec3& normal, const Vec3& lightDir, float shininess) noexcept
{
    Vec3 lightColor(255, 255, 255);
    Vec3 reflectDir = normalize(normal * (2.0f * dot(normal, lightDir)) - lightDir);
    float specAngle = std::max(dot(reflectDir, viewDir), 0.0f);
    float specularIntensity = std::pow(specAngle, shininess);
    return lightColor * specularIntensity;
}

bool Render::launchShadowRay(const Vec3& lightPos, const Vec3& hitPoint, const Vec3& lightDir, const Vec3& normal, int objectIndex) noexcept
{
    Hit shadowHit;
    float epsilon = 1e-4 * ((1.0f > hitPoint.length()) ? 1.0f : hitPoint.length());
    Ray shadowRay(hitPoint + normal * epsilon, lightDir);
    float distToLight = norm(lightPos - hitPoint);

    if (this->bvh.intersect(shadowRay, shadowHit) == true) {
        const auto& hitMaterial = this->scene.getObjects()[shadowHit.ObjectIdx]->getMaterial();
        if (hitMaterial->transmission > 0.0f)
            return false;

        if (shadowHit.t > 0.0f && shadowHit.t < distToLight)
            return true;
    }
    return false;
}

Vec3 computeRefraction(const Vec3& incidentDir, const Vec3& normal, float refractionRatio) noexcept {
    float cosTheta = std::min(dot(-incidentDir, normal), 1.0f);
    Vec3 refractedPerp = (incidentDir + normal * cosTheta) * refractionRatio;
    Vec3 refractedParallel = normal * -std::sqrt(std::abs(1.0f - dot(refractedPerp, refractedPerp)));
    return refractedPerp + refractedParallel;
}

const float PI = 3.14159265359f;

float computeNormalDistributionGGX(const Vec3 &normal,const Vec3 &halfVector, float roughness) noexcept
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;
    float normalDotHalf = std::max(dot(normal, halfVector), 0.0f);
    float normalDotHalfSq = normalDotHalf * normalDotHalf;

    float numerator = alphaSq;
    float denominator = (normalDotHalfSq * (alphaSq - 1.0f) + 1.0f);
    denominator = PI * denominator * denominator;

    return numerator / denominator;
}

float computeGeometrySchlickGGX(float normalDotView, float roughness) noexcept
{
    float roughnessBias = (roughness + 1.0f);
    float k = (roughnessBias * roughnessBias) / 8.0f;

    float numerator = normalDotView;
    float denominator = normalDotView * (1.0f - k) + k;

    return numerator / denominator;
}

float computeGeometrySmith(const Vec3 &normal, const Vec3 &viewDir, const Vec3 &lightDir, float roughness) noexcept
{
    float normalDotView = std::max(dot(normal, viewDir), 0.0f);
    float normalDotLight = std::max(dot(normal, lightDir), 0.0f);
    float geometryView = computeGeometrySchlickGGX(normalDotView, roughness);
    float geometryLight = computeGeometrySchlickGGX(normalDotLight, roughness);

    return geometryLight * geometryView;
}

Vec3 computeFresnelSchlick(float cosTheta, const Vec3 &baseReflectivity) noexcept
{
    float clampedCos = std::clamp(1.0f - cosTheta, 0.0f, 1.0f);
    float c2 = clampedCos * clampedCos;
    float c4 = c2 * c2;
    return baseReflectivity + (Vec3(1.0f, 1.0f, 1.0f) - baseReflectivity) * (c4 * clampedCos);
}

Vec3 Render::applyPBR(Ray& ray, Hit& minHit, const Vec3& albedoNorm) noexcept
{
    const auto& material = this->scene.getObjects()[minHit.ObjectIdx]->getMaterial();
    Vec3 totalRadiance = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 ambientRadiance = Vec3(0.0f, 0.0f, 0.0f);

    Vec3 hitPoint = minHit.position;
    Vec3 normal = minHit.normal;
    if (dot(normal, ray.dir) > 0.0f) 
        normal = normal * -1.0f;
    
    Vec3 viewDir = normalize(-ray.dir);
    Vec3 baseReflectivity = lerp(Vec3(0.04f, 0.04f, 0.04f), albedoNorm, material->metallic);

    for (const auto& light : this->scene.getLights()) 
    {
        if (!light->castsShadows()) {
            ambientRadiance += light->getRadiance(hitPoint) * albedoNorm;
            continue;
        }
        int numSamples = 16;
        float unblockedSamples = 0.0f;

        Vec3 centerLightDir = normalize(light->getCenter() - hitPoint); 

        for (int i = 0; i < numSamples; i++) {
            Vec3 jitteredLightPos = light->getSamplePosition();
            Vec3 jitteredLightDir = normalize(jitteredLightPos - hitPoint);

            if (!launchShadowRay(jitteredLightPos, hitPoint + normal * 0.001f, jitteredLightDir, normal, minHit.ObjectIdx)) {
                unblockedSamples += 1.0f;
            }
        }

        float shadowFactor = unblockedSamples / (float)numSamples;

        if (shadowFactor == 0.0f)
            continue;
        Vec3 incomingRadiance = light->getRadiance(hitPoint); 

        Vec3 halfVector = normalize(viewDir + centerLightDir); 
        float normalDistrib = computeNormalDistributionGGX(normal, halfVector, material->roughness);   
        float geometryTerm  = computeGeometrySmith(normal, viewDir, centerLightDir, material->roughness);      
        Vec3 fresnelTerm    = computeFresnelSchlick(std::max(dot(halfVector, viewDir), 0.0f), baseReflectivity);

        Vec3 specularNumerator = fresnelTerm * normalDistrib * geometryTerm;
        
        float NdotV = std::max(dot(normal, viewDir), 0.001f);
        float NdotL = std::max(dot(normal, centerLightDir), 0.001f);
        float specularDenominator = 4.0f * NdotV * NdotL; 
        
        Vec3 specularBRDF = specularNumerator / specularDenominator;

        Vec3 diffuseContrib = (Vec3(1.0f, 1.0f, 1.0f) - fresnelTerm) * (1.0f - material->metallic); 
        
        totalRadiance += (diffuseContrib * (albedoNorm / PI) + specularBRDF) * incomingRadiance * NdotL * shadowFactor;
    }

    return ambientRadiance + totalRadiance;
}

sf::Color Render::shade(Ray& ray, Hit& hit, int depth) noexcept
{
    if (depth <= 0)
        return sf::Color(0, 0, 0, 255);

    const auto& object = this->scene.getObjects()[hit.ObjectIdx];
    const auto& material = object->getMaterial();

    Vec3 albedo = object->getShape()->getColor();
    if (material->textureIndex > -1) {
        Vec2 uv = object->getShape()->getUv(hit.position);
        albedo = material->textureManager.getTexturePix(material->textureIndex, uv);
    }

    Vec3 albedoNorm = albedo / 255.0f;
    Vec3 finalColorNorm(0.0f, 0.0f, 0.0f);

    if (material->transmission == 0.0f) {
        finalColorNorm = applyPBR(ray, hit, albedoNorm); 
        if (material->roughness < 0.3f) {
            Vec3 viewDir = normalize(-ray.dir);
            Vec3 normal = hit.normal;

            Vec3 reflectDir = normalize(reflect(normalize(ray.dir), normal));
            Ray reflectedRay(hit.position + normal * 0.001f, reflectDir);
            Hit reflectHit;
            Vec3 bounceColor(0.1f, 0.1f, 0.1f);

            if (this->bvh.intersect(reflectedRay, reflectHit)) {
                sf::Color reflectColorSf = this->shade(reflectedRay, reflectHit, depth - 1);
                bounceColor = Vec3(reflectColorSf.r, reflectColorSf.g, reflectColorSf.b) / 255.0f;
            }

            Vec3 baseReflectivity = Vec3(0.04f, 0.04f, 0.04f);
            baseReflectivity = lerp(baseReflectivity, albedoNorm, material->metallic);
            
            float cosTheta = std::max(dot(normal, viewDir), 0.0f);
            Vec3 fresnelTerm = computeFresnelSchlick(cosTheta, baseReflectivity);

            Vec3 reflectionContrib = bounceColor * fresnelTerm * (1.0f - material->roughness);
            finalColorNorm += reflectionContrib;
        }
    }
    else {
        int glassDepth = std::min(depth, 5); 

        if (glassDepth <= 0) return sf::Color(0, 0, 0, 255);

        float refractionRatio = hit.frontFace ? (1.0f / material->ior) : material->ior;
        Vec3 unitIncidentDir = normalize(ray.dir);
        
        float cosTheta = std::min(dot(-unitIncidentDir, hit.normal), 1.0f);
        float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

        bool totalInternalReflection = refractionRatio * sinTheta > 1.0f; 
        
        float schlickBase = (1.0f - refractionRatio) / (1.0f + refractionRatio);
        schlickBase = schlickBase * schlickBase;
        float schlickFresnel = schlickBase + (1.0f - schlickBase) * std::pow((1.0f - cosTheta), 5.0f);

        Vec3 reflectDir = reflect(unitIncidentDir, hit.normal);
        Vec3 refractDir = computeRefraction(unitIncidentDir, hit.normal, refractionRatio);

        Ray reflectedRay(hit.position + hit.normal * 0.001f, reflectDir);
        Hit reflectHit;
        Vec3 reflectColor(0.1f, 0.1f, 0.1f);
        if (this->bvh.intersect(reflectedRay, reflectHit)) {
            sf::Color reflectColorSf = this->shade(reflectedRay, reflectHit, glassDepth - 1);
            reflectColor = Vec3(reflectColorSf.r, reflectColorSf.g, reflectColorSf.b) / 255.0f;
        }

        Vec3 finalGlassColor;

        if (totalInternalReflection) {
            finalGlassColor = reflectColor; 
        } else {
            Ray refractedRay(hit.position + refractDir * 0.001f, refractDir);
            Hit refractHit;
            Vec3 refractColor(0.1f, 0.1f, 0.1f); 
            if (this->bvh.intersect(refractedRay, refractHit)) {
                sf::Color refractColorSf = this->shade(refractedRay, refractHit, glassDepth - 1);
                refractColor = Vec3(refractColorSf.r, refractColorSf.g, refractColorSf.b) / 255.0f;
            }
            
            finalGlassColor = reflectColor * schlickFresnel + refractColor * (1.0f - schlickFresnel);
        }
        
        finalColorNorm = finalGlassColor * albedoNorm; 
    }

    finalColorNorm = finalColorNorm * 255.0f;
    finalColorNorm.x = std::clamp(finalColorNorm.x, 0.0f, 255.0f);
    finalColorNorm.y = std::clamp(finalColorNorm.y, 0.0f, 255.0f);
    finalColorNorm.z = std::clamp(finalColorNorm.z, 0.0f, 255.0f);

    return sf::Color(finalColorNorm.x, finalColorNorm.y, finalColorNorm.z, 255);
}