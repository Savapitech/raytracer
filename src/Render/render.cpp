#include "render.hpp"
#include "RayTracer.hpp"

#include <cmath>
#include <SFML/Graphics.hpp>

#define WIDTH 1920
#define HEIGHT 1080

Render::Render(scene::Scene &scene) noexcept
    : scene(scene),
      bvh(scene.getObjects()),
      window(sf::VideoMode(WIDTH, HEIGHT), "Raytracer"),
      RayBuffer(scene.getCamera().width * scene.getCamera().height * 4, 100),
      ImageRender(false),
      distance(0),
      load("./Asset/loading.png", scene.getCamera().width,scene.getCamera().height)
{
    Log::Logger::info("Window Open");
}

void Render::createRayBuffer(void) noexcept
{
    const auto &cam = scene.getCamera();
    this->ImageRender = true;
    sf::Clock clock;

    int nbPixel = cam.width * cam.height;
    int count = 0;
    int percent = 0;

    for (int x = 0; x < cam.width; x++){
        //if (load.pushLoad(window) == true)
        //    this->HandleWindow(true);
        if (window.isOpen() == false)
            return;
        if (percent != (count * 100) / nbPixel){
            percent = (count * 100) / nbPixel;
            this->load.pushPercent(window, percent);
        }
        for (int y = 0; y < cam.height; y++){
            this->fillRayBuffer(x, y);
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

void Render::HandleWindow(bool clear) noexcept{
    sf::Event event;

    while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window.close();
        }        
    this->window.clear({0, 0, 0, 255});
    this->window.draw(sprite);
    this->window.display();
    std::fill(RayBuffer.begin(), RayBuffer.end(), 100);
}

void Render::RunRender(void) noexcept
{
    Log::Logger::info("Start Render");
    
    while (this->window.isOpen()) {
        if (this->ImageRender == false){
            this->bvh.BuildSpacePartitionning();
            Log::Logger::info("Push new buffer");
            this->image.saveToFile("Legend.png");
        }
        scene::Camera cam = this->scene.getCamera();
        this->scene.updateCamera({cam.pos.x + static_cast<float>(0.1),cam.pos.y, cam.pos.z });
        this->createRayBuffer();

        this->HandleWindow(true);
    }
    Log::Logger::info("Window Close");
}