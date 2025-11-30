#include "render.hpp"
#include "bvh.hpp"

#define R(x) (x)
#define G(x) (x + 1)
#define B(x) (x + 2)
#define A(x) (x + 3)

void Render::writePixel(int x, int y, sf::Color color)
{
    int acutalPixel = (y * scene.getCamera().width + x) * 4;

    RayBuffer[R(acutalPixel)] = color.r;
    RayBuffer[G(acutalPixel)] = color.g;
    RayBuffer[B(acutalPixel)] = color.b;
    RayBuffer[A(acutalPixel)] = color.a;
}

Vec3 speculaire(const Vec3& V, const Vec3& N, const Vec3& L, float alpha)
{
    Vec3 lightColor(255, 255, 255);
    Vec3 R = normalize( N * (2.0f * dot(N, L)) - L );
    float specAngle = std::max(dot(R, V), 0.0f);
    float spec = std::pow(specAngle, alpha);
    return lightColor * spec;
}

bool Render::ShadowRay(Vec3 &light, Hit &minHit, Vec3 &P, Vec3 &L)
{
    Ray shadowRay(P + minHit.normal * 0.001f, L);
    shadowRay.minHit = 0.001f;
    shadowRay.maxHit = norm(light - P) - 0.001f;

    Hit tmp;

    for (auto& obj : scene.getObjects()) {
        if (obj.get() == minHit.object)
            continue;
        if (obj->shape->intersect(shadowRay, tmp))
            if (tmp.t > shadowRay.minHit && tmp.t < shadowRay.maxHit){
                //std::cout << "Shadow Ray: " << tmp.t << std::endl;
                return true;
            }
    }
    return false;
}

sf::Color Render::shade(Ray &ray, Hit &minHit)
{
    Vec3 light(10, 0, 10);
    Vec3 colorShape = minHit.object->shape->color;

    Vec3 P = ray.origin + ray.dir * minHit.t;
    Vec3 L = normalize(light - P);
    Vec3 N = normalize(P - minHit.object->shape->pos);


    if (ShadowRay(light, minHit, P, L) == true)
        return sf::Color::Black;

    
    float diff = std::max(dot(N, L), 0.0f);

    Vec3 spec = speculaire(normalize(-ray.dir), N, L, 50.f);
    
    Vec3 finalColor = spec + (colorShape * diff);

    finalColor.x = std::clamp(finalColor.x, 0.0f, 255.0f);
    finalColor.y = std::clamp(finalColor.y, 0.0f, 255.0f);
    finalColor.z = std::clamp(finalColor.z, 0.0f, 255.0f);

    return sf::Color(finalColor.x, finalColor.y, finalColor.z, 255);
}

void Render::FindObject(int x, int y)
{
    Ray ray(scene.getCamera(), x, y);

    Hit minHit;
    bool found = false;

    for (auto& obj : scene.getObjects())
    {
        Hit tmpHit;
        if (obj->shape->intersect(ray, tmpHit) == true)
        {
            if (tmpHit.t > 0 && tmpHit.t < minHit.t)
            {
                minHit = tmpHit;
                minHit.object = obj.get();
                found = true;
            }
        }
    }
    if (found == true) {
        sf::Color color = this->shade(ray, minHit);
        writePixel(x, y, color);
    }
}

void Render::StartRender(void)
{
    const auto &cam = scene.getCamera();
    this->ImageRender = true;

    for (int x = 0; x < cam.width; x++)
        for (int y = 0; y < cam.height; y++){
               this->FindObject(x, y);
        }
    this->image.create(scene.getCamera().width, scene.getCamera().height, RayBuffer.data());
    this->texture.loadFromImage(image);
    this->sprite.setTexture(this->texture);
}