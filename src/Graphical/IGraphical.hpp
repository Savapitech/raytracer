#pragma once

#include "Scene.hpp"

class IGraphical 
{
    public:
        virtual void display(void) = 0;
        virtual bool isOpen(void) = 0;
        virtual void addSprite(sf::Sprite &) = 0;

        virtual void handleEvent(void) = 0;
        virtual bool handleMovement(scene::Scene &) = 0;
};
