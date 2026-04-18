#include "Render.hpp"
#include "Ray.hpp"

void Render::writePixel(int x, int y, sf::Color color) noexcept
{
    int acutalPixel = (y * scene.getCamera().width + x) * 4;

    _frameBuffer[R(acutalPixel)] = color.r;
    _frameBuffer[G(acutalPixel)] = color.g;
    _frameBuffer[B(acutalPixel)] = color.b;
    _frameBuffer[A(acutalPixel)] = 255;
}

void Render::fillRayBuffer(float offsetX, float offsetY, int x, int y) noexcept
{
    float dirX = (2.0f * (offsetX + 0.5f) * _invWidth - 1.0f) * _aspect * _scale;
    float dirY = (1.0f - 2.0f * (offsetY + 0.5f) * _invHeight) * _scale;

    Vec3 pixelDir = normalize(scene.getCamera().forward + scene.getCamera().right * dirX + scene.getCamera().up * dirY);
    Ray ray(scene.getCamera().pos, pixelDir);
    Hit minHit;

    this->bvh.intersect(ray, minHit);
    if (minHit.ObjectIdx != -1) {
        sf::Color color = this->shade(ray, minHit, 1000);
        writePixel(x, y, color);
    }
    else
        writePixel(x, y, {0, 0, 0, 255});
}