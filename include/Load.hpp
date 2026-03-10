#pragma once

#include <SFML/Graphics.hpp>

#define FPS_60 160

class GLoad
{
    public:
        GLoad(std::string path, int widht, int height);
        
        bool pushLoad(sf::RenderWindow &window);
        void setFrame(int index);
        void pushPercent(sf::RenderWindow &window, int percent);

    private:
        int frameWidth;
        int frameHeight;
        int count;
        int columns;
        sf::Font font;
        sf::Text percent;
        sf::Texture textLoad;
        sf::Sprite spriteLoad;
        sf::Clock clock;

};