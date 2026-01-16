#include "RayTracer.hpp"

void Render::StartRenderRT(void) noexcept
{
    const auto &cam = scene.getCamera();
    this->ImageRender = true;
    sf::Clock clock;

    int nbPixel = cam.width * cam.height;
    int count = 0;
    int percent = 0;

    for (int x = 0; x < cam.width; x++){
        if (load.pushLoad(window) == true)
            this->HandleWindow(true);
        if (window.isOpen() == false)
            return;
        if (percent != (count * 100) / nbPixel){
            percent = (count * 100) / nbPixel;
            this->load.pushPercent(window, percent);
        }
        for (int y = 0; y < cam.height; y++){
            this->FindObject(x, y);
            count++;
        }
    }
    std::cout << "End\n";
    sf::Time RenderTime = clock.getElapsedTime();
    sf::Int32 RenderTimeMs = RenderTime.asMilliseconds();
    std::cout << CLR_BOLD_DEBUG << "Render Time:" << RenderTimeMs << CLR_RESET << std::endl;

    this->image.create(scene.getCamera().width, scene.getCamera().height, RayBuffer.data());
    this->texture.loadFromImage(image);
    this->sprite.setTexture(this->texture);
}

void Render::RunRenderRT(void) noexcept
{   
    while (this->window.isOpen()) {
        if (this->ImageRender == false){
            this->bvh.BuildSpacePartitionning();
            Log::Logger::info("Push new buffer");
            this->StartRender();
        }
        this->HandleWindow(true);
    }
    Log::Logger::info("Window Close");
}


void RayTracer::runRT(void) 
    {
        Log::Logger::info("Start Running Game...");
        this->render.RunRenderRT();   
    }