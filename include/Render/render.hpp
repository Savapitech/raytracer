#pragma once

#include <memory>
#include <vector>
#include "Load.hpp"
#include "Scene.hpp"
#include <SFML/Graphics.hpp>

#define R(x) (x)
#define G(x) (x + 1)
#define B(x) (x + 2)
#define A(x) (x + 3)

class Render
{
    public:
        void InitRender(void) noexcept;
        void StartRender(void) noexcept;
        void FindObject(int x, int y) noexcept;
        void writePixel(int x, int y, sf::Color color) noexcept;
        sf::Color shade(Ray &ray, Hit &hit) noexcept;
        bool ShadowRay(Vec3 &light, Hit &hit, Vec3 &P, Vec3 &L) noexcept;
        void HandleWindow(void) noexcept;
        Vec3 AppliedFong(Ray &ray, Hit &minHit) noexcept;
        Render(const scene::Scene &scene) noexcept;

    private:
        const scene::Scene& scene;
        BVH bvh;
        sf::RenderWindow window;
        std::vector<sf::Uint8> RayBuffer;
        bool ImageRender;
        float distance;
        sf::Image image;
        sf::Texture texture;
        sf::Sprite sprite;

        GLoad load;     
};

