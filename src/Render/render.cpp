#include "render.hpp"

#include <cmath>
#include <SFML/Graphics.hpp>

#define WIDTH 1920
#define HEIGHT 1080

Render::Render(const scene::Scene &scene)
    : scene(scene),
      bvh(scene.getObjects()),
      window(sf::VideoMode(WIDTH, HEIGHT), "Raytracer"),
      RayBuffer(scene.getCamera().width * scene.getCamera().height * 4, 100),
      ImageRender(false),
      distance(0)
{
    Log::Logger::info("Window Open");
}

void Render::InitRender(void)
{
    Log::Logger::info("Start Render");
    sf::Event event;
    
    while (this->window.isOpen()) {
        if (this->ImageRender == false){
            this->bvh.BuildSpacePartitionning();
            Log::Logger::info("Push new buffer");
            this->StartRender();
        }
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window.close();
        }
        this->window.clear();
        this->window.draw(sprite);
        this->window.display();
    }
    Log::Logger::info("Window Close");
}