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
    int samplesPerPixel = 64;
    Vec3 accumulatedLight(0.0f, 0.0f, 0.0f);

    for (int s = 0; s < samplesPerPixel; ++s) {
        float jitterX = fastRandomFloat(-0.5f, 0.5f);
        float jitterY = fastRandomFloat(-0.5f, 0.5f);

        float dirX = (2.0f * (offsetX + jitterX + 0.5f) * _invWidth - 1.0f) * _aspect * _scale;
        float dirY = (1.0f - 2.0f * (offsetY + jitterY + 0.5f) * _invHeight) * _scale;

        Vec3 pixelDir = normalize(scene.getCamera().forward + scene.getCamera().right * dirX + scene.getCamera().up * dirY);
        Ray ray(scene.getCamera().pos, pixelDir);
        Hit minHit;

        this->bvh.intersect(ray, minHit);
        
        if (minHit.ObjectIdx != -1) {
            if (this->_isPathTracing) {
                accumulatedLight += this->shade<true>(ray, minHit, 5);
            } else {
                accumulatedLight += this->shade<false>(ray, minHit, 5);
            }
        } else {
            //accumulatedLight += Vec3(0.5f, 0.7f, 1.0f); Coord UV for the background 
        }
    }
    Vec3 averageLight = accumulatedLight / (float)samplesPerPixel;

    averageLight.x = averageLight.x / (averageLight.x + 1.0f);
    averageLight.y = averageLight.y / (averageLight.y + 1.0f);
    averageLight.z = averageLight.z / (averageLight.z + 1.0f);

    averageLight = averageLight * 255.0f;

    sf::Color finalColor(
        std::clamp((int)averageLight.x, 0, 255),
        std::clamp((int)averageLight.y, 0, 255),
        std::clamp((int)averageLight.z, 0, 255),
        255
    );

    writePixel(x, y, finalColor);
}