#include "render.hpp"
#include "RayTracer.hpp"

#include <cmath>
#include <SFML/Graphics.hpp>

#define WIDTH 1920
#define HEIGHT 1080

Render::Render(scene::Scene &scene) noexcept
    : scene(scene),
      bvh(scene.getObjects()),
      RayBuffer(scene.getCamera().width * scene.getCamera().height * 4, 0),
      ImageRender(false),
      distance(0),
      texture((sf::Vector2u){WIDTH, HEIGHT}),
      sprite(texture)
      
{
    for (int i = 3; i + 4 < scene.getCamera().width * scene.getCamera().height * 4; i+=4){
        RayBuffer[i] = 255;
    }
    Log::Logger::info("Window Open");
}

#define SAMPLING 1
#include <random>

void Render::skipPixels(void) noexcept
{
    if (count_change == 0){
        binary_sample = 3;
        count_change++;
    } else if (count_change == 1){
        binary_sample = 1;
        count_change++;
    } else if (count_change == 2){
        binary_sample = 0;
        count_change++;
    } else if (count_change == 4){
        this->ImageRender = true;
        return;
    }
}

void Render::createRayBuffer(void) noexcept
{
    const auto &cam = scene.getCamera();
   
    sf::Clock timeFrameBufferBuild;

    int nbPixel = cam.width * cam.height;
    int count = 0;
    int percent = 0;

    skipPixels();
    if (this->ImageRender == true)
        return;

    /*===========Clean Frame Buffer===========*/
    std::fill(RayBuffer.begin(), RayBuffer.end(), 0);

    /*===Fill the pixel into the frame buffer===*/
    for (int x = 0;x < cam.width; x++){
        for (int y = 0; y < cam.height; y++){
            if ((x | y) & binary_sample)
                continue;
            this->fillRayBuffer(0.0f + x, 0.0f + y, x, y);
            count++;
        }
    }

    /*==============Render Time==============*/
    sf::Time RenderTime = timeFrameBufferBuild.getElapsedTime();
    int32_t RenderTimeMs = RenderTime.asMilliseconds();
    std::cout << CLR_BOLD_DEBUG << "Render Time:" << RenderTimeMs << CLR_RESET << std::endl;
    
    /*==============Update The Frame Buffer==============*/
    this->texture.update(RayBuffer.data());
    this->sprite.setTexture(this->texture);
    count_change++;
}

/*
**Build the bvh, create a ray buffer and push it into the context
*/
void Render::RunRender(void) noexcept
{
    Log::Logger::info("Start Render");

    this->bvh.BuildSpacePartitionning();
    
    this->gr.addSprite(this->sprite);

    while (this->gr.isOpen()) {
        if (this->ImageRender == false){
            this->createRayBuffer();
            this->gr.display();
            Log::Logger::info("Push new buffer");
            this->image.saveToFile("IronMan.png");
        }
        this->gr.handleEvent();
        if (this->gr.handleMovement(this->scene) == true){
            this->ImageRender = false;
            count_change = 0;
        }
    }
    Log::Logger::info("Window Close");
}
