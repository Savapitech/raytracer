#include "Sfml.hpp"

Sfml::Sfml(std::uint16_t width, std::uint16_t height) :
  _window(sf::VideoMode({width, height}), "Raytracer")
{
}

void Sfml::display()
{
    _window.clear();
    for (sf::Sprite &sprite : _spriteTab)
        _window.draw(sprite);
    _window.display();
}

bool Sfml::isOpen(void)
{
    return _window.isOpen();
}

void Sfml::addSprite(sf::Sprite &sprite)
{
    _spriteTab.push_back(sprite);
}

void Sfml::handleEvent()
{
    while (const std::optional event = _window.pollEvent())
        if (event->is<sf::Event::Closed>())
            _window.close();
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
