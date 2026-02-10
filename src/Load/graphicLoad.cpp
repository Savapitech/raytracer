#include "render.hpp"

GLoad::GLoad(std::string path, int widht, int height)
{
    this->textLoad.loadFromFile(path);
    this->spriteLoad.setTexture(textLoad);
    this->spriteLoad.setTextureRect(sf::IntRect(0, 0, 32, 32));
    this->spriteLoad.setPosition(sf::Vector2f(widht / 2, height / 2));
    this->spriteLoad.setOrigin(sf::Vector2f(32 / 2, 32 / 2));
    this->font.loadFromFile("./Asset/font/VCR_OSD_MONO_1.001.ttf");
    this->percent.setFont(font);
    this->percent.setString("0%");
    sf::FloatRect bounds = percent.getLocalBounds();
    this->percent.setOrigin(bounds.left + bounds.width / 2.0f,bounds.top + bounds.height / 2.0f);
    this->percent.setCharacterSize(32);
    this->percent.setPosition(sf::Vector2f(widht / 2 + 64, height / 2));
    this->columns = 12;
    this->frameWidth  = 32;
    this->frameHeight = 32;
    this->count = 0;
}

void GLoad::setFrame(int index)
{
    int x = (index % columns) * frameWidth;
    int y = (index / columns) * frameHeight;

    if (this->count == 12)
        this->count = 0;
    this->spriteLoad.setTextureRect(sf::IntRect(x, y, frameWidth, frameHeight));
}

void GLoad::pushPercent(sf::RenderWindow &window, int percent)
{
    this->percent.setString(std::to_string(percent) + "%");
    window.draw(this->percent);
}



bool GLoad::pushLoad(sf::RenderWindow &window){
    if (clock.getElapsedTime().asMilliseconds() > FPS_60 / 2){
        this->setFrame(this->count);
        clock.restart();
        window.draw(this->percent);
        window.draw(this->spriteLoad);
        count++;
        return true;
    }
    return false;
}