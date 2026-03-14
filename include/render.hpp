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

#define WIDTH 1920
#define HEIGHT 1080

class Graphical 
{
    public:
        Graphical(void);
        void display(void);
        bool isOpen(void);
        void addSprite(sf::Sprite &);

        void handleEvent(void);
        bool handleMovement(scene::Scene &);
    private:
        sf::RenderWindow window;
        std::vector<std::reference_wrapper<sf::Sprite>> spriteTab;
};

class Render
{
    public:
        void RunRender(void) noexcept;
        void createRayBuffer(void) noexcept;
        void skipPixels() noexcept;
        void fillRayBuffer(float offsetX, float offsetY, int x, int y) noexcept;
        void fillTile(int startX, int startY);
        void writePixel(int x, int y, sf::Color color) noexcept;
        sf::Color shade(Ray &ray, Hit &minHit, int depth) noexcept;
        bool ShadowRay(const Vec3 &light, Vec3 &P, Vec3 &L, Vec3 &N, int index) noexcept;
        Vec3 AppliedFong(Ray &ray, Hit &minHit, const Vec3& albedo) noexcept;
        Render(scene::Scene &scene) noexcept;

    private:
        scene::Scene& scene;
        BVH bvh;
        std::vector<uint8_t> RayBuffer;
        bool ImageRender;
        float distance;
             
        sf::Image image;
        sf::Texture TframeBuffer;
        sf::Sprite SframeBuffer;

        Graphical gr;

        int count_change = 0;
        int binary_sample = 0;
        
        float aspect;
        float scale;
        float invWidth;
        float invHeight;
};

