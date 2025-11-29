#pragma once

#include <memory>
#include <vector>
#include "Scene.hpp"
#include <SFML/Graphics.hpp>


class Render
{
    public:
        void StartRender(void);
        Render(const std::vector<std::unique_ptr<Object>> &objects);
    private:
        const std::vector<std::unique_ptr<Object>> &objects;
        sf::RenderWindow window;
};

