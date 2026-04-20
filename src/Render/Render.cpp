#include "Render.hpp"
#include "Ray.hpp"

#include <cmath>
#include <SFML/Graphics.hpp>

#define WIDTH 1920
#define HEIGHT 1080

Render::Render(scene::Scene &scene) noexcept
    : scene(scene),
      bvh(scene.getObjects()),
      _frameBuffer(scene.getCamera().width * scene.getCamera().height * 4, 0),
      _imageIsRender(false),
      _TframeBuffer((sf::Vector2u){WIDTH, HEIGHT}),
      _SframeBuffer(_TframeBuffer)
      
{
    for (int i = 3; i + 4 < scene.getCamera().width * scene.getCamera().height * 4; i+=4){
        _frameBuffer[i] = 255;
    }
    this->_aspect = float(scene.getCamera().width) / float(scene.getCamera().height);
    this->_scale  = tanf((scene.getCamera().fov * 0.5f) * (M_PI / 180.0f));
    this->_invWidth  = 1.0f / scene.getCamera().width;
    this->_invHeight = 1.0f / scene.getCamera().height;
    Log::Logger::info("Window Open");
}

void Render::skipPixels(void) noexcept
{
    /*===Set a binary sample for skipping pixel in case of movement===*/
    switch (_countChange) {
        case 0:
            _binarySample = 3;
            _countChange++;
            break;
        case 1:
            _binarySample = 1;
            _countChange++;
            break;
        case 2:
            _binarySample = 0;
            _countChange++;
            break;
        default:
            this->_imageIsRender = true;
            return;
    }
}
 
void Render::fillTile(int startX, int startY)
{
    /*===Creation of tile to access the L1 cache===*/
    const auto &cam = scene.getCamera();

    int tileSize = 16;

    int endX = std::min(startX + tileSize, cam.width);
    int endY = std::min(startY + tileSize, cam.height);

    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            if ((x | y) & _binarySample)
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
    if (this->_imageIsRender == true){
        return;
    }
 
    
    /*===========Prep to send tile of Window===========*/
    int width = cam.width;
    int height = cam.height;
 
    int tileSize = 16;
 
    /*===========File tole of Window for better access to L1===========*/
    int totalTiles = ((width + tileSize - 1) / tileSize) * ((height + tileSize - 1) / tileSize);
 
    for (int ty = 0; ty < height; ty += tileSize) {
        for (int tx = 0; tx < width; tx += tileSize) {
            fillTile(tx, ty);
            count++;
            int newPercent = (count * 100) / totalTiles;
            if (newPercent != percent) {
                percent = newPercent;
                std::cout << "\rRender: " << percent << "%" << std::flush;
            }
        }
    }
    std::cout << "\rRender: 100%" << std::endl;
 
    /*==============Render Time==============*/
    sf::Time RenderTime = timeFrameBufferBuild.getElapsedTime();
    int32_t RenderTimeMs = RenderTime.asMilliseconds();
    std::cout << CLR_BOLD_DEBUG << "Render Time:" << RenderTimeMs << CLR_RESET << std::endl;
    
    /*==============Update The Frame Buffer==============*/
    this->_TframeBuffer.update(_frameBuffer.data());
    this->_SframeBuffer.setTexture(this->_TframeBuffer);
    _countChange++;
}

/*
**Build the bvh, create a ray buffer and push it into the context
*/
void Render::runRender(void) noexcept
{
    Log::Logger::info("Start Render");

    /*===Build the binary space three===*/
    this->bvh.buildSpacePartitionning();
    
    /*===add frame buffer sprite to the pool of sprite to draw===*/
    this->_gr.addSprite(this->_SframeBuffer);
    
    /*===Start the main loop===*/
    while (this->_gr.isOpen()) {

        /*===Check the if we need to rebuild the frame buffer in case of movement===*/
        if (this->_imageIsRender == false){
            this->createRayBuffer();
            this->_gr.display();
            Log::Logger::info("Push new buffer");
        }

        /*===Check window event for closing the window===*/
        this->_gr.handleEvent();

        /*===Change the camera data in case of inputs===*/
        if (this->_gr.handleMovement(this->scene) == true){
            this->_imageIsRender = false;
            std::fill(this->_frameBuffer.begin(), this->_frameBuffer.end(), 0);
            _countChange = 0;
        }
    }
    Log::Logger::info("Window Close");
}
