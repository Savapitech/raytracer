#pragma once

#include <memory>
#include <vector>
#include "Scene.hpp"
#include <SFML/Graphics.hpp>


class Render
{
    public:
        void InitRender(void);
        void StartRender(void);
        void FindObject(int x, int y);
        void writePixel(int x, int y, sf::Color color);
        sf::Color shade(Ray &ray, Hit &hit);
        bool ShadowRay(Vec3 &light, Hit &hit, Vec3 &P, Vec3 &L);
        Render(const scene::Scene &scene);

    private:
        float a = 15;
        const scene::Scene& scene;
        sf::RenderWindow window;
        std::vector<sf::Uint8> RayBuffer;
        bool ImageRender;
        float distance;
        sf::Image image;
        sf::Texture texture;
        sf::Sprite sprite;
       
};

