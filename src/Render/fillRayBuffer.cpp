#include "render.hpp"
#include "RayTracer.hpp"

#include <cmath>

void Render::writePixel(int x, int y, sf::Color color) noexcept
{
    int acutalPixel = (y * scene.getCamera().width + x) * 4;

    RayBuffer[R(acutalPixel)] = color.r;
    RayBuffer[G(acutalPixel)] = color.g;
    RayBuffer[B(acutalPixel)] = color.b;
    RayBuffer[A(acutalPixel)] = color.a;
}

void Render::fillRayBuffer(int x, int y) noexcept
{
    Ray ray(scene.getCamera(), x, y);
    Hit minHit;

    this->bvh.intersect(ray, minHit);
    if (minHit.ObjectIdx != -1) {
        sf::Color color = this->shade(ray, minHit);
        writePixel(x, y, color);
    }
}