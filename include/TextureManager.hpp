#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#define FPS_60 160

class TextureManager
{
    public:
        TextureManager() = default;
        int uploadTexture(std::string Path)
            {
                for (size_t i = 0; i != TexturePathPool.size(); i++)
                    if (Path == TexturePathPool[i])
                        return i;
                
                sf::Image texture;
                if (texture.loadFromFile(Path) == false)
                    throw std::runtime_error("Error");
                TexturePathPool.push_back(Path);
                TexturePool.push_back(texture);
                return TexturePool.size() - 1;
            }

        Vec3 getTexturePix(int index, Vec2 uv)
        {
            if (index < 0 || index >= (int)TexturePool.size())
                return Vec3(1.0f, 0.0f, 1.0f); 
            const sf::Image& img = TexturePool[index];
            auto size = img.getSize();
    
            if (size.x == 0 || size.y == 0) 
                return Vec3(0,0,0);

            unsigned int x = (int)(uv.x * size.x) % size.x;
            unsigned int y = (int)(uv.y * size.y) % size.y;

            if (x < 0) 
                x += size.x;
            if (y < 0) 
                y += size.y;
        
            sf::Color color = img.getPixel({x, y});
            return Vec3(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
        }

    private:
        std::vector<std::string> TexturePathPool;
        std::vector<sf::Image> TexturePool;
};