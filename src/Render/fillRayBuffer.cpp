#include "render.hpp"
#include "RayTracer.hpp"

#include <cmath>
#define SAMPLING 1

void Render::writePixel(int x, int y, sf::Color color) noexcept
{
    int acutalPixel = (y * scene.getCamera().width + x) * 4;

    RayBuffer[R(acutalPixel)] = color.r;
    RayBuffer[G(acutalPixel)] = color.g;
    RayBuffer[B(acutalPixel)] = color.b;
    RayBuffer[A(acutalPixel)] = 255;
}

void Render::fillRayBuffer(float offsetX, float offsetY, int x, int y) noexcept
{
    float dirx = (2.0f * (offsetX + 0.5f) * invWidth - 1.0f) * aspect * scale;
    float diry = (1.0f - 2.0f * (offsetY + 0.5f) * invHeight) * scale;

    Vec3 pixelDir = normalize(scene.getCamera().forward + scene.getCamera().right * dirx + scene.getCamera().up * diry);

    Ray ray(scene.getCamera().pos, pixelDir);
    Hit minHit;

    this->bvh.intersect(ray, minHit);
    if (minHit.ObjectIdx != -1) {
        sf::Color color = this->shade(ray, minHit);
        writePixel(x, y, color);
    }
    else
        writePixel(x, y, {0, 0, 0, 0});
}