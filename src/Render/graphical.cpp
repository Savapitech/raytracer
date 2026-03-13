#include "render.hpp"

Graphical::Graphical(void)  :
    window(sf::VideoMode({WIDTH, HEIGHT}), "Raytracer")
    {

    }

void Graphical::display()
{
    this->window.clear();
    for (sf::Sprite &sprite : this->spriteTab)
        this->window.draw(sprite);
    this->window.display();
}

bool Graphical::isOpen(void)
{
    return window.isOpen();
}

void Graphical::addSprite(sf::Sprite &sprite)
{
    this->spriteTab.push_back(sprite);
}

void Graphical::handleEvent()
{
    while (const std::optional event = this->window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            this->window.close();
    }            
}

bool Graphical::handleMovement(scene::Scene &scene)
{
    scene::Camera cam = scene.getCamera();
    bool move = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){
        scene.updateCamera({cam.pos.x + static_cast<float>(-0.2),cam.pos.y, cam.pos.z });
        move = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))    {
        scene.updateCamera({cam.pos.x + static_cast<float>(0.2),cam.pos.y, cam.pos.z });
        move = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))    {
        move = true;
        scene.updateCamera({cam.pos.x, cam.pos.y, cam.pos.z  + static_cast<float>(-0.2)});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))    {
        move = true;
        scene.updateCamera({cam.pos.x ,cam.pos.y, cam.pos.z + static_cast<float>(0.2) });
    }
    return move;
}