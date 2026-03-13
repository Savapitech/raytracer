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

int count_change = 0;
int binary_sample = 0;

#define SAMPLING 1
#include <random>

void Render::createRayBuffer(void) noexcept
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
    const auto &cam = scene.getCamera();
   
    sf::Clock clock;/*SFML*/

    int nbPixel = cam.width * cam.height;
    int count = 0;
    int percent = 0;

    std::fill(RayBuffer.begin(), RayBuffer.end(), 0);

    for (int i = 0; i != SAMPLING; i++){
        float offsetX = 0.0;
        float offsetY = 0.0;
        for (int x = 0;x < cam.width; x++){
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
    int32_t RenderTimeMs = RenderTime.asMilliseconds();/*SFML*/
    std::cout << CLR_BOLD_DEBUG << "Render Time:" << RenderTimeMs << CLR_RESET << std::endl;
    //this->image.create(scene.getCamera().width, scene.getCamera().height, RayBuffer.data());/*SFML*/
    //this->texture.loadFromImage(image);/*SFML*/
    
    this->texture.update(RayBuffer.data());
    this->sprite.setTexture(this->texture);/*SFML*/
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

            /*SFML*/
        }
        this->gr.handleEvent();
        if (this->gr.handleMovement(this->scene) == true){
            this->ImageRender = false;
            count_change = 0;
        }
      
    }
    Log::Logger::info("Window Close");
}
