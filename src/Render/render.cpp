#include "render.hpp"

#include <cmath>
#include <SFML/Graphics.hpp>

void Render::HandleWindow(void) noexcept{
    sf::Event event;

    while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window.close();
        }
    
    this->window.draw(sprite);
    this->window.display();
    this->window.clear();
}

void Render::InitRender(void) noexcept
{
    Log::Logger::info("Start Render");
    
    while (this->window.isOpen()) {
        if (this->ImageRender == false){
            this->bvh.BuildSpacePartitionning();
            Log::Logger::info("Push new buffer");
            this->StartRender();
        }
        this->HandleWindow();
    }
    Log::Logger::info("Window Close");
}