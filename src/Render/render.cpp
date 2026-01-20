#include "render.hpp"

#include <cmath>
#include <SFML/Graphics.hpp>

void Render::HandleWindow(bool clear) noexcept {
    if (clear)
        this->window.clear();
    this->window.draw(sprite);
    this->window.display();
    sf::Event event;
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            this->window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            this->window.close();
    }
}

#include <SFML/Config.hpp>

void Render::RunRender(void) noexcept
{
    Log::Logger::info("Start Render");
    
    while (this->window.isOpen()) {
        if (this->ImageRender == false){
            this->bvh.BuildSpacePartitionning();
            Log::Logger::info("Push new buffer");
            this->StartRender();
        }
        //sf::Time t3 = sf::seconds(0.01f);
//
        //sf::sleep(sl);
        this->HandleWindow(true);

    }
    Log::Logger::info("Window Close");
}