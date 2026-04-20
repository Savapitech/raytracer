#pragma once

#include <memory>
#include <vector>
//#include "Load.hpp"
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
        void runRender(void) noexcept;
        void createRayBuffer(void) noexcept;
        void skipPixels() noexcept;
        void fillRayBuffer(float offsetX, float offsetY, int x, int y) noexcept;
        void fillTile(int startX, int startY);
        void writePixel(int x, int y, sf::Color color) noexcept;
        sf::Color shade(Ray &ray, Hit &minHit, int depth) noexcept;
        bool launchShadowRay(const Vec3& lightPos, const Vec3& hitPoint, const Vec3& lightDir, const Vec3& normal, int objectIndex) noexcept;
        Vec3 applyPBR(Ray &ray, Hit &minHit, const Vec3& albedo) noexcept;
        Render(scene::Scene &scene) noexcept;

    private:
        scene::Scene& scene;
        BVH bvh;
        std::vector<uint8_t> _frameBuffer;
        bool _imageIsRender;
             
        sf::Image   _image;
        sf::Texture _TframeBuffer;
        sf::Sprite  _SframeBuffer;

        Graphical _gr;

        int _countChange = 0;
        int _binarySample = 0;
        
        float _aspect;
        float _scale;
        float _invWidth;
        float _invHeight;
};

