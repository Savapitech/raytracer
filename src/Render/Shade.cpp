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

bool Render::launchShadowRay(const Vec3& lightPos, const Vec3& hitPoint, const Vec3& lightDir, const Vec3& normal) noexcept
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

template <bool IsPathTracer>
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

    if constexpr (IsPathTracer)
    {
        for (const auto& light : this->scene.getLights())
        {
            if (!light->castsShadows())
                continue;
            Vec3 lightSamplePos = light->getSamplePosition();
            Vec3 centerLightDir = normalize(light->getCenter() - hitPoint);
            Vec3 lightDir = normalize(lightSamplePos - hitPoint);

            if (launchShadowRay(lightSamplePos, hitPoint + normal * 0.001f, lightDir, normal))
                continue;

            Vec3 incomingRadiance = light->getRadiance(hitPoint);

            float NdotL = std::max(dot(normal, centerLightDir), 0.0f);

            if (NdotL <= 0.0f)
                continue;

            Vec3 halfVector = normalize(viewDir + centerLightDir);
            float normalDistrib = computeNormalDistributionGGX(normal, halfVector, material->roughness);
            float geometryTerm = computeGeometrySmith(normal, viewDir, centerLightDir, material->roughness);
            Vec3 fresnelTerm = computeFresnelSchlick(std::max(dot(halfVector, viewDir), 0.0f), baseReflectivity);

            Vec3 specularBRDF = (fresnelTerm * normalDistrib * geometryTerm)
                              / std::max(4.0f * std::max(dot(normal, viewDir), 0.001f) * NdotL, 0.05f);

            Vec3 diffuseContrib = (Vec3(1.0f, 1.0f, 1.0f) - fresnelTerm) * (1.0f - material->metallic);

            totalRadiance += (diffuseContrib * (albedoNorm / PI) + specularBRDF)
                           * incomingRadiance * NdotL;
        }
        return totalRadiance;
    }
    else
    {
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

                if (!launchShadowRay(jitteredLightPos, hitPoint + normal * 0.001f,
                                     jitteredLightDir, normal))
                    unblockedSamples += 1.0f;
            }

            float shadowFactor = unblockedSamples / (float)numSamples;

            if (shadowFactor == 0.0f)
                continue;

            Vec3  incomingRadiance = light->getRadiance(hitPoint);
            Vec3  halfVector = normalize(viewDir + centerLightDir);
            float normalDistrib = computeNormalDistributionGGX(normal, halfVector, material->roughness);
            float geometryTerm = computeGeometrySmith(normal, viewDir, centerLightDir, material->roughness);
            Vec3  fresnelTerm = computeFresnelSchlick(std::max(dot(halfVector, viewDir), 0.0f), baseReflectivity);

            float NdotV = std::max(dot(normal, viewDir), 0.001f);
            float NdotL = std::max(dot(normal, centerLightDir), 0.001f);

            Vec3 specularBRDF = (fresnelTerm * normalDistrib * geometryTerm)
                              / std::max(4.0f * NdotV * NdotL, 0.05f);

            Vec3 diffuseContrib = (Vec3(1.0f, 1.0f, 1.0f) - fresnelTerm) * (1.0f - material->metallic);

            totalRadiance += (diffuseContrib * (albedoNorm / PI) + specularBRDF) * incomingRadiance * NdotL * shadowFactor;
        }
        return ambientRadiance + totalRadiance;
    }
}

template <bool IsPathTracer>
Vec3 Render::shade(Ray& ray, Hit& hit, int depth) noexcept
{
    
    if (depth <= 0)
        return Vec3(0, 0, 0);

    const auto& object = this->scene.getObjects()[hit.ObjectIdx];
    const auto& material = object->getMaterial();

    Vec2 uv = object->getShape()->getUv(hit.position);
    Vec3 albedo;
    switch (object->getMaterial()->textureType)
    {
        case TextureType::CHESSBOARD:
            albedo = ProceduralTexture::getChessboard(uv, material->color, material->colorChess, matData.frequency);
            break;
        case TextureType::LOAD_IMAGE :
            albedo = material->textureManager.getTexturePix(material->textureIndex, uv);
            break;
        default:
            albedo = object->getShape()->getColor();
            break;
    }

    Vec3 albedoNorm = albedo / 255.0f;
    Vec3 finalColorNorm(0.0f, 0.0f, 0.0f);

    if constexpr (IsPathTracer == false)
    {
        Log::Logger::debug("Start Ray Tracing");
        
    if (material->transmission == 0.0f) {
        finalColorNorm = applyPBR<false>(ray, hit, albedoNorm); 
        if (material->roughness < 0.3f) {
            Vec3 viewDir = normalize(-ray.dir);
            Vec3 normal = hit.normal;

            Vec3 reflectDir = normalize(reflect(normalize(ray.dir), normal));
            Ray reflectedRay(hit.position + normal * 0.001f, reflectDir);
            Hit reflectHit;
            Vec3 bounceColor(0.1f, 0.1f, 0.1f);

            if (this->bvh.intersect(reflectedRay, reflectHit)) {
                bounceColor = this->shade<false>(reflectedRay, reflectHit, depth - 1);
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

        if (glassDepth <= 0) 
            return Vec3(0, 0, 0);

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
            reflectColor = this->shade<false>(reflectedRay, reflectHit, glassDepth - 1);
        }

        Vec3 finalGlassColor;

        if (totalInternalReflection) {
            finalGlassColor = reflectColor; 
        } else {
            Ray refractedRay(hit.position + refractDir * 0.001f, refractDir);
            Hit refractHit;
            Vec3 refractColor(0.1f, 0.1f, 0.1f); 
            if (this->bvh.intersect(refractedRay, refractHit)) {
                refractColor = this->shade<false>(refractedRay, refractHit, glassDepth - 1);
            }
            
            finalGlassColor = reflectColor * schlickFresnel + refractColor * (1.0f - schlickFresnel);
        }
        
        finalColorNorm = finalGlassColor * albedoNorm; 
        }
    }
    else 
    {
        if (material->transmission > 0.0f) {
            float refractionRatio = hit.frontFace ? (1.0f / material->ior) : material->ior;
            Vec3 unitIncidentDir = normalize(ray.dir);

            float cosTheta = std::min(dot(-unitIncidentDir, hit.normal), 1.0f);
            float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

            bool cannotRefract = refractionRatio * sinTheta > 1.0f; 
            
            float reflectance = 1.0f;
            if (!cannotRefract) {
                float r0 = (1.0f - refractionRatio) / (1.0f + refractionRatio);
                r0 = r0 * r0;
                reflectance = r0 + (1.0f - r0) * std::pow((1.0f - cosTheta), 5.0f);
            }

            Vec3 bounceDir;
            Vec3 bounceOrigin;

            if (cannotRefract || fastRandomFloat(0.0f, 1.0f) < reflectance) {
                Vec3 reflected = reflect(unitIncidentDir, hit.normal);
                bounceDir = normalize(reflected + randomUnitVector() * material->roughness);
                bounceOrigin = hit.position + hit.normal * 0.001f; 
            } else {
                Vec3 refracted = computeRefraction(unitIncidentDir, hit.normal, refractionRatio);
                bounceDir = normalize(refracted + randomUnitVector() * material->roughness);
                bounceOrigin = hit.position - hit.normal * 0.001f;
            }

            Ray bounceRay(bounceOrigin, bounceDir);
            Hit bounceHit;
            if (this->bvh.intersect(bounceRay, bounceHit)) {
                Vec3 glassCol = this->shade<true>(bounceRay, bounceHit, depth - 1);
                finalColorNorm = albedoNorm * glassCol;
            } else {
                finalColorNorm = Vec3(0.0f, 0.0f, 0.0f);; //multiplication par coordoné U V du monde et y aura un bon résultat
            }
        } 
        else {
            Vec3 directLighting = applyPBR<true>(ray, hit, albedoNorm);
            Vec3 indirectLighting(0.0f, 0.0f, 0.0f);
            
            Vec3 unitIncidentDir = normalize(ray.dir);
            Vec3 viewDir = -unitIncidentDir;
            Vec3 baseReflectivity = lerp(Vec3(0.04f, 0.04f, 0.04f), albedoNorm, material->metallic);
            float cosTheta = std::max(dot(hit.normal, viewDir), 0.0f);
            Vec3 fresnelTerm = computeFresnelSchlick(cosTheta, baseReflectivity);
            float specProbability = std::clamp((fresnelTerm.x + fresnelTerm.y + fresnelTerm.z) / 3.0f, 0.0f, 1.0f);
            
            Vec3 bounceDir;
            Vec3 bounceOrigin = hit.position + hit.normal * 0.001f;
            Vec3 reflectionFilter(1.0f, 1.0f, 1.0f);

            if (fastRandomFloat(0.0f, 1.0f) < specProbability) {
                Vec3 reflected = reflect(unitIncidentDir, hit.normal);
                bounceDir = normalize(reflected + randomUnitVector() * material->roughness);
                if (dot(bounceDir, hit.normal) < 0.0f) {
                    bounceDir = bounceDir - hit.normal * dot(bounceDir, hit.normal) * 2.0f;
                }
                reflectionFilter = lerp(Vec3(1.0f, 1.0f, 1.0f), albedoNorm, material->metallic);
            } 
            else {
                bounceDir = normalize(hit.normal + randomUnitVector());
                reflectionFilter = albedoNorm;
            }

            Ray bounceRay(bounceOrigin, bounceDir);
            Hit bounceHit;
            if (this->bvh.intersect(bounceRay, bounceHit)) {
                Vec3 bounceCol = this->shade<true>(bounceRay, bounceHit, depth - 1);
                indirectLighting = reflectionFilter * bounceCol;
            } else
                indirectLighting = reflectionFilter * Vec3(0.1f, 0.1f, 0.1f);; //* Vec3(0.5f, 0.7f, 1.0f); // multiplication par coordoné U V du monde et y aura un bon résultat
            finalColorNorm = directLighting + indirectLighting;
        }
    }
    if (finalColorNorm.x > 1) finalColorNorm.x = 1;
    if (finalColorNorm.y > 1) finalColorNorm.y = 1;
    if (finalColorNorm.z > 1) finalColorNorm.z = 1;
    return finalColorNorm;
}


template Vec3 Render::shade<true>(Ray&, Hit&, int) noexcept;
template Vec3 Render::shade<false>(Ray&, Hit&, int) noexcept;

template Vec3 Render::applyPBR<true>(Ray&, Hit&, const Vec3&) noexcept;
template Vec3 Render::applyPBR<false>(Ray&, Hit&, const Vec3&) noexcept;