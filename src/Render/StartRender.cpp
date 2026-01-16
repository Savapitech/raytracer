#include "render.hpp"
#include "RayTracer.hpp"

void Render::writePixel(int x, int y, sf::Color color) noexcept
{
    int acutalPixel = (y * scene.getCamera().width + x) * 4;

    RayBuffer[R(acutalPixel)] = color.r;
    RayBuffer[G(acutalPixel)] = color.g;
    RayBuffer[B(acutalPixel)] = color.b;
    RayBuffer[A(acutalPixel)] = color.a;
}

void Render::FindObject(int x, int y) noexcept
{
    Ray ray(scene.getCamera(), x, y);
    Hit minHit;

    this->bvh.intersect(ray, minHit);
    if (minHit.ObjectIdx != -1) {
        sf::Color color = this->shade(ray, minHit);
        writePixel(x, y, color);
    }
}

void Render::StartRender(void) noexcept
{
    const auto &cam = scene.getCamera();
    this->ImageRender = true;
    sf::Clock clock;

    int nbPixel = cam.width * cam.height;
    int count = 0;
    int percent = 0;

    for (int x = 0; x < cam.width; x++){
        if (load.pushLoad(window) == true)
            this->HandleWindow(true);
        if (window.isOpen() == false)
            return;
        if (percent != (count * 100) / nbPixel){
            percent = (count * 100) / nbPixel;
            this->load.pushPercent(window, percent);
        }
        for (int y = 0; y < cam.height; y++){
            this->FindObject(x, y);
            count++;
        }
    }
    std::cout << "End\n";
    sf::Time RenderTime = clock.getElapsedTime();
    sf::Int32 RenderTimeMs = RenderTime.asMilliseconds();
    std::cout << CLR_BOLD_DEBUG << "Render Time:" << RenderTimeMs << CLR_RESET << std::endl;

    this->image.create(scene.getCamera().width, scene.getCamera().height, RayBuffer.data());
    this->texture.loadFromImage(image);
    this->sprite.setTexture(this->texture);
}
