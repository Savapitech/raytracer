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

sf::Color Render::shade(Ray &ray, Hit &minHit)
{
    Vec3 light(0, 0, this->a);
    Vec3 colorShape(0, 0, 255);

    Vec3 P = ray.origin + ray.dir * minHit.t;
    Vec3 L = normalize(light - P);
    Vec3 N = normalize(P - minHit.object->shape->pos);

    float diff = std::max(dot(N, L), 0.0f);

    Vec3 finalColor = colorShape * diff;

    finalColor.x = std::clamp(finalColor.x, 0.0f, 255.0f);
    finalColor.y = std::clamp(finalColor.y, 0.0f, 255.0f);
    finalColor.z = std::clamp(finalColor.z, 0.0f, 255.0f);
    return sf::Color(finalColor.x, finalColor.y, finalColor.z, 255);
}

void Render::FindObject(int x, int y)
{
    Ray ray(scene.getCamera(), x, y); /*Les Ray sont dans la bonne direction*/

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