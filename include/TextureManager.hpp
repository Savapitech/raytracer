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
                return Vec3(255.0f, 255.0f, 255.0f) / 255; 
            const sf::Image& img = TexturePool[index];
            auto size = img.getSize();
    
            if (size.x == 0 || size.y == 0) 
                return Vec3(0,0,0);

            int x = (int)(uv.x * size.x) % (int)size.x;
            int y = (int)(uv.y * size.y) % (int)size.y;
                    
            if (x < 0) 
                x += (int)size.x;
            if (y < 0) 
                y += (int)size.y;
                    
            sf::Color color = img.getPixel({(unsigned int)x, (unsigned int)y});
            return Vec3(color.r, color.g, color.b) / 255;
        }

    private:
        std::vector<std::string> TexturePathPool;
        std::vector<sf::Image> TexturePool;
};

namespace ProceduralTexture {

    inline Vec3 getChessboard(Vec2 uv, Vec3 color1, Vec3 color2, float freq) {
        float s = std::floor(uv.x * freq);
        float t = std::floor(uv.y * freq);

        if (std::fmod(s + t, 2.0f) == 0.0f) {
            return color1;
        }
        return color2;
    }
}

enum class TextureType {
    NONE,
    CHESSBOARD,
    LOAD_IMAGE,
};