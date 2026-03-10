#include "render.hpp"
/*SFML*/
GLoad::GLoad(std::string path, int width, int height)
    : percent(font),
      spriteLoad(textLoad) 
{
    if (!this->textLoad.loadFromFile(path)) {
    }

    this->spriteLoad.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
    
    float fW = static_cast<float>(width);
    float fH = static_cast<float>(height);
    this->spriteLoad.setPosition({fW / 2.0f, fH / 2.0f});
    this->spriteLoad.setOrigin({16.0f, 16.0f});

    if (!this->font.openFromFile("./Asset/font/VCR_OSD_MONO_1.001.ttf")) {
    }

    this->percent.setString("0%");
    this->percent.setCharacterSize(32);

    sf::FloatRect bounds = percent.getLocalBounds();
    this->percent.setOrigin({bounds.position.x + bounds.size.x / 2.0f, 
                             bounds.position.y + bounds.size.y / 2.0f});
    
    this->percent.setPosition({fW / 2.0f + 64.0f, fH / 2.0f});

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
    sf::IntRect rect({x, y},{frameWidth, frameHeight});
    this->spriteLoad.setTextureRect(rect);
}
void GLoad::pushPercent(sf::RenderWindow &window, int percent)
{
    this->percent.setString(std::to_string(percent) + "%");/*SFML*/
    window.draw(this->percent);/*SFML*/
}


/*SFML*/
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