#include "Sfml.hpp"
#include "Render.hpp"

Sfml::Sfml(void)  :
    window(sf::VideoMode({WIDTH, HEIGHT}), "Raytracer")
    {

    }

void Sfml::display()
{
    this->window.clear();
    for (sf::Sprite &sprite : this->spriteTab)
        this->window.draw(sprite);
    this->window.display();
}

bool Sfml::isOpen(void)
{
    return window.isOpen();
}

void Sfml::addSprite(sf::Sprite &sprite)
{
    this->spriteTab.push_back(sprite);
}

void Sfml::handleEvent()
{
    while (const std::optional event = this->window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            this->window.close();
    }            
}

bool Sfml::handleMovement(scene::Scene &scene)
{
    scene::Camera cam = scene.getCamera();
    bool move = false;
    float rotSpeed = 0.05f;

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
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    {
        move = true;
        scene.updateCamera({cam.pos.x ,cam.pos.y + static_cast<float>(0.2), cam.pos.z});
    }
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))    {
        move = true;
        scene.updateCamera({cam.pos.x ,cam.pos.y - static_cast<float>(0.2), cam.pos.z});
    }
   if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad6)) {
        Vec3 newDir;
        newDir.x = cam.dir.x * cos(-rotSpeed) - cam.dir.z * sin(-rotSpeed);
        newDir.y = cam.dir.y;
        newDir.z = cam.dir.x * sin(-rotSpeed) + cam.dir.z * cos(-rotSpeed);
        
        scene.updateCameraDir(newDir);
        move = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad4)) {
        Vec3 newDir;
        newDir.x = cam.dir.x * cos(rotSpeed) - cam.dir.z * sin(rotSpeed);
        newDir.y = cam.dir.y;
        newDir.z = cam.dir.x * sin(rotSpeed) + cam.dir.z * cos(rotSpeed);
        
        scene.updateCameraDir(newDir);
        move = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad8)) {
        Vec3 newDir = cam.dir * cos(rotSpeed) + cam.up * sin(rotSpeed);
        scene.updateCameraDir(newDir);
        move = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad5)) {
        Vec3 newDir = cam.dir * cos(-rotSpeed) + cam.up * sin(-rotSpeed);
        scene.updateCameraDir(newDir);
        move = true;
    }
    return move;
}
