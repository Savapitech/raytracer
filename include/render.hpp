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
        void RunRender(void) noexcept;
        void createRayBuffer(void) noexcept;
        void fillRayBuffer(float offsetX, float offsetY, int x, int y) noexcept;
        void writePixel(int x, int y, sf::Color color) noexcept;
        sf::Color shade(Ray &ray, Hit &minHit) noexcept;
        bool ShadowRay(Vec3 &light, Vec3 &P, Vec3 &L) noexcept;
        void HandleWindow(bool clear) noexcept;
        Vec3 AppliedFong(Ray &ray, Hit &minHit) noexcept;
        Render(scene::Scene &scene) noexcept;

    private:
        scene::Scene& scene;
        BVH bvh;
        sf::RenderWindow window;
        std::vector<uint8_t> RayBuffer;
        bool ImageRender;
        float distance;

        GLoad load;
             
        sf::Image image;
        sf::Texture texture;
        sf::Sprite sprite;

};

