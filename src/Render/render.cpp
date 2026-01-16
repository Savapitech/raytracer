#include "render.hpp"

#include <cmath>
#include <SFML/Graphics.hpp>

void Render::HandleWindow(bool clear) noexcept{
    sf::Event event;

    while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window.close();
        }
    
    this->window.draw(sprite);
    this->window.display();
    if (clear)
        this->window.clear();
}

void Render::RunRender(void) noexcept
{
    Log::Logger::info("Start Render");
    
    while (this->window.isOpen()) {
        if (this->ImageRender == false){
            this->bvh.BuildSpacePartitionning();
            Log::Logger::info("Push new buffer");
            this->StartRender();
        }
        this->HandleWindow(true);
    }
    Log::Logger::info("Window Close");
}