#include <SFML/Graphics.hpp>
#include <atomic>
#include <chrono>
#include <cmath>
#include <thread>

#include "Render.hpp"
#include "Sfml.hpp"

Render::Render(scene::Scene &scene, const CmdConfig::config_t &config) noexcept
    : scene(scene),
      bvh(scene.getObjects()),
      _frameBuffer(scene.getCamera().width * scene.getCamera().height * 4, 0),
      _imageIsRender(false),
      _config(config),
      _isPathTracing(config.pathtracing),
      _TframeBuffer(sf::Vector2u(WIDTH, HEIGHT)),
      _SframeBuffer(_TframeBuffer)

{
    for (int i = 3; i + 4 < scene.getCamera().width * scene.getCamera().height * 4; i+=4)
        _frameBuffer[i] = 255;
    this->_aspect = float(scene.getCamera().width) / float(scene.getCamera().height);
    this->_scale  = tanf((scene.getCamera().fov * 0.5f) * (M_PI / 180.0f));
    this->_invWidth  = 1.0f / scene.getCamera().width;
    this->_invHeight = 1.0f / scene.getCamera().height;
    this->_gr = new Sfml(WIDTH, HEIGHT);
    Log::Logger::info("Window Open");
}

void Render::fillTile(int startX, int startY)
{
    /*===Creation of tile to access the L1 cache===*/
    const auto &cam = scene.getCamera();

    int endX = std::min(startX + TILE_SIZE, cam.width);
    int endY = std::min(startY + TILE_SIZE, cam.height);

    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            fillRayBuffer((float)x, (float)y, x, y);
        }
    }
}

void Render::createRayBuffer(void) noexcept
{
    const auto &cam = scene.getCamera();
    int width  = cam.width;
    int height = cam.height;

    int tilesX   = (width  + TILE_SIZE - 1) / TILE_SIZE;
    int tilesY   = (height + TILE_SIZE - 1) / TILE_SIZE;
    int numTiles = tilesX * tilesY;

    /*===========Dispatch tiles across CPU threads===========*/
    std::atomic<int> tileIdx{0};
    std::atomic<int> doneTiles{0};

    int numThreads = (int)std::thread::hardware_concurrency() - 1;
    if (!numThreads)
        numThreads = 1;

    Log::Logger::info("Rendering with " + std::to_string(numThreads) + " threads");

    auto worker = [&]() {
        int tile;
        while ((tile = tileIdx.fetch_add(1, std::memory_order_relaxed)) < numTiles) {
            int tx = (tile % tilesX) * TILE_SIZE;
            int ty = (tile / tilesX) * TILE_SIZE;
            fillTile(tx, ty);
            doneTiles.fetch_add(1, std::memory_order_release);
        }
    };

    std::vector<std::thread> threads;
    threads.reserve(numThreads);
    for (int i = 0; i < numThreads; i++)
        threads.emplace_back(worker);

    sf::Clock timer;
    auto lastDisplay = std::chrono::steady_clock::now();

    while (doneTiles.load(std::memory_order_acquire) < numTiles) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDisplay).count();

        if (elapsed >= DISPLAY_INTERVAL_MS) {
            this->_TframeBuffer.update(_frameBuffer.data());
            this->_SframeBuffer.setTexture(this->_TframeBuffer);
            this->_gr->display();
            this->_gr->handleEvent();
            lastDisplay = now;
        }
    }

    for (auto &t : threads)
        t.join();

    /*==============Render Time==============*/
    int32_t renderTimeMs = timer.getElapsedTime().asMilliseconds();
    Log::Logger::info("Render Time: " + std::to_string(renderTimeMs) + "ms");

    /*==============Final frame buffer push==============*/
    this->_TframeBuffer.update(_frameBuffer.data());
    this->_SframeBuffer.setTexture(this->_TframeBuffer);
    this->_gr->display();

    this->_imageIsRender = true;
}

/*
** Build the bvh, create a ray buffer and push it into the context
*/
void Render::runRender(void) noexcept
{
    Log::Logger::info("Start Render");

    /*===Build the binary space three===*/
    this->bvh.buildSpacePartitionning();

    /*===add frame buffer sprite to the pool of sprite to draw===*/
    this->_gr->addSprite(this->_SframeBuffer);

    /*===Start the main loop===*/
    while (this->_gr->isOpen()) {

        /*===Check the if we need to rebuild the frame buffer in case of movement===*/
        if (this->_imageIsRender == false){
            this->createRayBuffer();
            Log::Logger::info("Render complete");
        }

        /*===Check window event for closing the window===*/
        this->_gr->handleEvent();

        /*===Change the camera data in case of inputs===*/
        if (this->_gr->handleMovement(this->scene) == true){
            this->_imageIsRender = false;
            std::fill(this->_frameBuffer.begin(), this->_frameBuffer.end(), 0);
        }
    }
    Log::Logger::info("Window Close");
}
