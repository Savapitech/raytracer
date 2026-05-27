#pragma once

#include <vector>
#include <string>

#define FPS_60 160

#ifndef HEADLESS_BUILD
#include <SFML/Graphics.hpp>
#else
// Forward-declare the two stbi symbols we use.
// The implementation is compiled exactly once in stb_image_impl.cpp.
extern "C" {
unsigned char *stbi_load(const char *filename, int *x, int *y,
                         int *channels_in_file, int desired_channels);
void stbi_image_free(void *retval_from_stbi_load);
}
#endif

// -----------------------------------------------------------------------

class TextureManager
{
public:
    TextureManager() = default;

    int uploadTexture(std::string path)
    {
        for (size_t i = 0; i != _texturePathPool.size(); i++)
            if (path == _texturePathPool[i])
                return static_cast<int>(i);

#ifndef HEADLESS_BUILD
        // --- SFML path ---
        sf::Image texture;
        if (!texture.loadFromFile(path))
            throw std::runtime_error("TextureManager: cannot load " + path);
        _texturePathPool.push_back(path);
        _texturePool.push_back(texture);
#else
        // --- stb_image path ---
        int w = 0, h = 0, ch = 0;
        unsigned char *data = stbi_load(path.c_str(), &w, &h, &ch, 4);
        if (!data)
            throw std::runtime_error("TextureManager: cannot load " + path);
        HeadlessTex tex;
        tex.w = w;
        tex.h = h;
        tex.pixels.assign(data, data + w * h * 4);
        stbi_image_free(data);
        _texturePathPool.push_back(path);
        _texturePool.push_back(std::move(tex));
#endif
        return static_cast<int>(_texturePool.size()) - 1;
    }

    Vec3 getTexturePix(int index, Vec2 uv)
    {
        if (index < 0 || index >= static_cast<int>(_texturePool.size()))
            return Vec3(1.0f, 1.0f, 1.0f);

#ifndef HEADLESS_BUILD
        // --- SFML path ---
        const sf::Image &img = _texturePool[index];
        auto size = img.getSize();
        if (size.x == 0 || size.y == 0)
            return Vec3(0, 0, 0);

        int x = static_cast<int>(uv.x * static_cast<float>(size.x)) % static_cast<int>(size.x);
        int y = static_cast<int>(uv.y * static_cast<float>(size.y)) % static_cast<int>(size.y);
        if (x < 0) x += static_cast<int>(size.x);
        if (y < 0) y += static_cast<int>(size.y);

        sf::Color color = img.getPixel({static_cast<unsigned>(x), static_cast<unsigned>(y)});
        return Vec3(color.r, color.g, color.b) / 255.0f;
#else
        // --- stb_image path ---
        const HeadlessTex &tex = _texturePool[index];
        if (tex.w == 0 || tex.h == 0)
            return Vec3(0, 0, 0);

        int x = static_cast<int>(uv.x * static_cast<float>(tex.w)) % tex.w;
        int y = static_cast<int>(uv.y * static_cast<float>(tex.h)) % tex.h;
        if (x < 0) x += tex.w;
        if (y < 0) y += tex.h;

        const unsigned char *px = tex.pixels.data() + (y * tex.w + x) * 4;
        return Vec3(px[0], px[1], px[2]) / 255.0f;
#endif
    }

private:
#ifndef HEADLESS_BUILD
    std::vector<std::string>  _texturePathPool;
    std::vector<sf::Image>    _texturePool;
#else
    struct HeadlessTex {
        int w = 0, h = 0;
        std::vector<unsigned char> pixels;
    };
    std::vector<std::string>   _texturePathPool;
    std::vector<HeadlessTex>   _texturePool;
#endif
};

namespace ProceduralTexture {

    inline Vec3 getChessboard(Vec2 uv, Vec3 color1, Vec3 color2, float freq) {
        float s = std::floor(uv.x * freq);
        float t = std::floor(uv.y * freq);
        if (std::fmod(s + t, 2.0f) == 0.0f)
            return color1;
        return color2;
    }
}

enum class TextureType {
    NONE,
    CHESSBOARD,
    LOAD_IMAGE,
};
