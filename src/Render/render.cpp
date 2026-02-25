#include "render.hpp"
#include "RayTracer.hpp"

#include <cmath>
#include <SFML/Graphics.hpp>

#define WIDTH 1920
#define HEIGHT 1080

Render::Render(scene::Scene &scene) noexcept
    : scene(scene),
      bvh(scene.getObjects()),
      window(sf::VideoMode(WIDTH, HEIGHT), "Raytracer"),
      RayBuffer(scene.getCamera().width * scene.getCamera().height * 4, 0),
      ImageRender(false),
      distance(0),
      load("./Asset/loading.png", scene.getCamera().width,scene.getCamera().height)
      
{
    for (int i = 3; i + 4 < scene.getCamera().width * scene.getCamera().height * 4; i+=4){
        RayBuffer[i] = 255;
    }
    texture.create(1920, 1080);
    Log::Logger::info("Window Open");
}

bool change = false;
int count_change = 0;
int binary_sample = 0;

#define SAMPLING 1
#include <random>

void Render::createRayBuffer(void) noexcept
{
    if (change == true)
        count_change = 0;
    if (count_change == 0)
        binary_sample = 3;
    if (count_change == 1)
        binary_sample = 1;
    if (count_change == 2)
        binary_sample = 0;
    if (count_change > 2)
        return;
    const auto &cam = scene.getCamera();
    this->ImageRender = true;
    sf::Clock clock;/*SFML*/

    int nbPixel = cam.width * cam.height;
    int count = 0;
    int percent = 0;

    for (int i = 0; i != SAMPLING; i++){
        float offsetX = 0.0;//dis(gen);
        float offsetY = 0.0;//dis(gen);
        for (int x = 0;x < cam.width; x++){
            //if (load.pushLoad(window) == true)
            //    this->HandleWindow(true);
            if (x & 7 && window.isOpen() == false)
                return;
            for (int y = 0; y < cam.height; y++){
                if ((x | y) & binary_sample)/*1 3 7 */ /*Sampling Hard Code */
                    continue;
                this->fillRayBuffer(offsetX + x, offsetY + y, x, y);
                count++;
            }
        }
    }
    //std::cout << "End\n";
    sf::Time RenderTime = clock.getElapsedTime();/*SFML*/
    sf::Int32 RenderTimeMs = RenderTime.asMilliseconds();/*SFML*/
    std::cout << CLR_BOLD_DEBUG << "Render Time:" << RenderTimeMs << CLR_RESET << std::endl;
    this->image.create(scene.getCamera().width, scene.getCamera().height, RayBuffer.data());/*SFML*/
    //this->texture.loadFromImage(image);/*SFML*/
    this->texture.update(RayBuffer.data());
    this->sprite.setTexture(this->texture);/*SFML*/
    count_change++;
    change = false;
}

void Render::HandleWindow(bool clear) noexcept{
    sf::Event event;

    while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window.close();
        }        
    this->window.clear({0, 0, 0, 255});
    this->window.draw(sprite);
    this->window.display();
    std::fill(RayBuffer.begin(), RayBuffer.end(), 100);
}

/*
**Build the bvh, create a ray buffer and push it into the context
*/
void Render::RunRender(void) noexcept
{
    Log::Logger::info("Start Render");
    
    while (this->window.isOpen()) {
        if (this->ImageRender == false){
            this->bvh.BuildSpacePartitionning();
            Log::Logger::info("Push new buffer");
            this->createRayBuffer();
            this->image.saveToFile("IronMan.png");

            /*SFML*/
        }
        scene::Camera cam = this->scene.getCamera();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
            this->scene.updateCamera({cam.pos.x + static_cast<float>(-0.2),cam.pos.y, cam.pos.z });
            change = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))    {
            this->scene.updateCamera({cam.pos.x + static_cast<float>(0.2),cam.pos.y, cam.pos.z });
            change = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))    {
            change = true;
            this->scene.updateCamera({cam.pos.x, cam.pos.y, cam.pos.z  + static_cast<float>(-0.2)});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))    {
            change = true;
            this->scene.updateCamera({cam.pos.x ,cam.pos.y, cam.pos.z + static_cast<float>(0.2) });
        }
        this->createRayBuffer();
        this->HandleWindow(true);
    }
    Log::Logger::info("Window Close");
}