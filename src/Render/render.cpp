#include "render.hpp"
#include <SFML/Graphics.hpp>

#define WIDTH 1920
#define HEIGHT 1080

Render::Render(const std::vector<std::unique_ptr<Object>>& objects)
    : objects(objects),
    window(sf::VideoMode(WIDTH, HEIGHT), "Raytracer")

{
    Log::Logger::info("Window Open");
}

void Render::StartRender(void)
{
    for (size_t i = 0; i < objects.size(); ++i) {
        objects[i]->shape->ShowShape();
    }
    Log::Logger::info("Start Render");
    while (this->window.isOpen()) {
        sf::Event event;
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window.close();
        }
        this->window.clear();
        this->window.display();
    }
    Log::Logger::info("Window Close");
}