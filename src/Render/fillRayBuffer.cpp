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
    Ray ray(scene.getCamera(), offsetX, offsetY);
    Hit minHit;

    this->bvh.intersect(ray, minHit);
    if (minHit.ObjectIdx != -1) {
        sf::Color color = this->shade(ray, minHit);
        writePixel(x, y, color);
    }
    else
        writePixel(x, y, {0, 0, 0, 0});
}