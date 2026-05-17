#pragma once

#include <vector>

#include "CmdConfig.hpp"
#include "IGraphical.hpp"
#include "Scene.hpp"



namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

namespace render {
    using namespace space;
    using namespace graphical;
    using namespace object;
    using namespace material;
    using namespace light;
    using namespace scene;
    using namespace shape;

#define R(x) (x)
#define G(x) (x + 1)
#define B(x) (x + 2)
#define A(x) (x + 3)

#define WIDTH 1920
#define HEIGHT 1080

class Render {
public:
  void runRender(void) noexcept;
  void createRayBuffer(void) noexcept;
  void fillRayBuffer(float offsetX, float offsetY, int x, int y) noexcept;
  void fillTile(int startX, int startY);
  void writePixel(int x, int y, const Vec3 &color) noexcept;

  template <bool IsPathTracer>
  Vec3 shade(Ray &ray, Hit &minHit, int depth) noexcept;

  template <bool IsPathTracer>
  Vec3 applyPBR(Ray &ray, Hit &minHit, const Vec3 &albedo) noexcept;

  bool launchShadowRay(const Vec3 &lightPos, const Vec3 &hitPoint,
                       const Vec3 &lightDir, const Vec3 &normal) noexcept;
  Render(scene::Scene &scene, const CmdConfig::config_t &config) noexcept;

  // Cluster helpers
  void buildBVH() noexcept { _bvh.buildSpacePartitionning(); }
  const std::vector<uint8_t> &getFrameBuffer() const noexcept {
    return _frameBuffer;
  }
  void renderSubset(const std::vector<uint32_t> &tileIndices, int tilesX,
                    int numThreads) noexcept;
  void resetFrameBuffer() noexcept {
    std::fill(_frameBuffer.begin(), _frameBuffer.end(), 0);
    for (size_t i = 3; i < _frameBuffer.size(); i += 4)
      _frameBuffer[i] = 255;
  }

private:
  scene::Scene &_scene;
  BVH _bvh;
  std::vector<uint8_t> _frameBuffer;
  bool _imageIsRender;
  const CmdConfig::config_t &_config;
  const bool _isPathTracing;

  sf::Image _image;
  sf::Texture _TframeBuffer;
  sf::Sprite _SframeBuffer;

  IGraphical *_gr;

#define TILE_SIZE 16
#define DISPLAY_INTERVAL_MS 4

  float _aspect;
  float _scale;
  float _invWidth;
  float _invHeight;
};

} // namespace render
