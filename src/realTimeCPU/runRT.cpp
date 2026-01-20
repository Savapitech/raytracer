#include "RayTracer.hpp"

void Render::StartRenderRT(void) noexcept
{
    const auto &cam = scene.getCamera();
    this->ImageRender = true;
    sf::Clock clock;

    for (int x = 0; x < cam.width; x++){
        if (window.isOpen() == false)
            return;
        for (int y = 0; y < cam.height; y++)
            this->FindObject(x, y);
    }

    this->image.create(scene.getCamera().width, scene.getCamera().height, RayBuffer.data());
    this->texture.loadFromImage(image);
    this->sprite.setTexture(this->texture);
}

/*Image Render va être set a false des qu'on bouge*/


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
    Log::Logger::info("Game Over...");
}