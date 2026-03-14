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
      TframeBuffer((sf::Vector2u){WIDTH, HEIGHT}),
      SframeBuffer(TframeBuffer)
      
{
    for (int i = 3; i + 4 < scene.getCamera().width * scene.getCamera().height * 4; i+=4){
        RayBuffer[i] = 255;
    }
    this->aspect = float(scene.getCamera().width) / float(scene.getCamera().height);
    this->scale  = tanf((scene.getCamera().fov * 0.5f) * (M_PI / 180.0f));
    this->invWidth  = 1.0f / scene.getCamera().width;
    this->invHeight = 1.0f / scene.getCamera().height;
    Log::Logger::info("Window Open");
}

void Render::skipPixels(void) noexcept
{
    /*===Set a binary sample for skipping pixel in case of movement===*/
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

void Render::fillTile(int startX, int startY)
{
    const auto &cam = scene.getCamera();

    int tileSize = 16;

    int endX = std::min(startX + tileSize, cam.width);
    int endY = std::min(startY + tileSize, cam.height);

    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            if ((x | y) & binary_sample)
                continue;
            fillRayBuffer((float)x, (float)y, x, y);
        }
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

    /*===========Prep to send tile of Window===========*/
    int width = cam.width;
    int height = cam.height;

    int tileSize = 16;

    /*===========File tole of Window for better access to L1===========*/
    for (int ty = 0; ty < height; ty += tileSize) {
        for (int tx = 0; tx < width; tx += tileSize) {
            fillTile(tx, ty);
        }
    }

    /*==============Render Time==============*/
    sf::Time RenderTime = timeFrameBufferBuild.getElapsedTime();
    int32_t RenderTimeMs = RenderTime.asMilliseconds();
    std::cout << CLR_BOLD_DEBUG << "Render Time:" << RenderTimeMs << CLR_RESET << std::endl;
    
    /*==============Update The Frame Buffer==============*/
    this->TframeBuffer.update(RayBuffer.data());
    this->SframeBuffer.setTexture(this->TframeBuffer);
    count_change++;
}

/*
**Build the bvh, create a ray buffer and push it into the context
*/
void Render::RunRender(void) noexcept
{
    Log::Logger::info("Start Render");

    /*===Build the binary space three===*/
    this->bvh.BuildSpacePartitionning();
    
    /*===add frame buffer sprite to the pool of sprite to draw===*/
    this->gr.addSprite(this->SframeBuffer);
    
    /*===Start the main loop===*/
    while (this->gr.isOpen()) {

        /*===Check the if we need to rebuild the frame buffer in case of movement===*/
        if (this->ImageRender == false){
            this->createRayBuffer();
            this->gr.display();
            Log::Logger::info("Push new buffer");
            this->image.saveToFile("IronMan.png");
        }

        /*===Check window event for closing the window===*/
        this->gr.handleEvent();

        /*===Change the camera data in case of inputs===*/
        if (this->gr.handleMovement(this->scene) == true){
            this->ImageRender = false;
            count_change = 0;
        }
    }
    Log::Logger::info("Window Close");
}
