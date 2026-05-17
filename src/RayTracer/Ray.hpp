#pragma once

#include "CmdConfig.hpp"
#include "Render.hpp"
#include "Scene.hpp"

class Ray {
public:
  Vec3 origin;
  Vec3 dir;
  float invDir[3];
  float minHit = 0.001f;
  float maxHit = 1e30f;

  Ray() = default;
  Ray(const scene::Camera &camera, float x, float y);
  Ray(const Vec3 &origin, const Vec3 &direction);
};

class Hit {
public:
  float t = 1e30f;
  Vec3 position;
  Vec3 normal;

  int ObjectIdx = -1;
  bool frontFace = false;
};


namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

using namespace render;
using namespace object;
using namespace space;
using namespace shape;
using namespace material;

namespace RayTracer {

class RayTracer {
public:
  RayTracer(const CmdConfig::config_t &config);
  void run();

private:
  scene::Scene _scene;
  Render _render;
};
} // namespace RayTracer