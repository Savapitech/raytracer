#include "render.hpp"

#define WIDTH 1920
#define HEIGHT 1080

Render::Render(const scene::Scene &scene) noexcept
    : scene(scene),
      bvh(scene.getObjects()),
      window(sf::VideoMode(WIDTH, HEIGHT), "Raytracer"),
      RayBuffer(scene.getCamera().width * scene.getCamera().height * 4, 100),
      ImageRender(false),
      distance(0),
      load("./Asset/loading.png", scene.getCamera().width,scene.getCamera().height)
{
    Log::Logger::info("Window Open");
}