#pragma once

#include "Scene.hpp"

class IGraphical
{
    public:
        virtual void display(void) = 0;
        virtual bool isOpen(void) = 0;
        virtual void addSprite(sf::Sprite &) = 0;

        virtual void handleEvent(void) = 0;
        virtual void handleGui(void) = 0;
        virtual bool handleMovement(scene::Scene &) = 0;

        virtual void save(const uint8_t *data, uint32_t width, uint32_t height) = 0;
        virtual bool needsLiveUpdate(void) const = 0;
};
