#include "ConeInf.hpp"

#include "Object.hpp"
#include "Ray.hpp"
#include <cmath>



namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

namespace shape {
    using namespace space;
    using namespace object;
    using namespace material;
    using namespace scene;

ConeInf::ConeInf(const libconfig::Setting &s) {
  _type = "ConeInf";
  _pos = scene::readVec3(s["pos"]);
  _radius = (float)s["radius"];
  _height = (float)s["height"];
  _color = scene::readVec3(s["color"]);
  if (s.exists("dir"))
    _dir = scene::readVec3(s["dir"]);
  else
    _dir = {1, 0, 0};
}

AABB ConeInf::getObjectAABB() const noexcept {
  float inf = 1e6f;
  return AABB({_pos.x - inf, _pos.y - inf, _pos.z - inf},
              {_pos.x + inf, _pos.y + inf, _pos.z + inf});
}

Vec3 ConeInf::getCentroid() const noexcept { return _pos; }

Vec2 ConeInf::getUv(Vec3 &) const noexcept { return {0, 0}; }

bool ConeInf::intersect(Ray &ray, Hit &hit) const noexcept {
  Vec3 center = _pos;
  Vec3 v = normalize(_dir);
  Vec3 origin = ray.origin - _pos;
  Vec3 rayDir = ray.dir;
  float alpha = (_radius / _height);
  float k = 1.0f + (alpha * alpha);

  float a = (dot(rayDir, rayDir) -
             ((1.0f + (k * k)) * (dot(rayDir, v) * dot(rayDir, v))));
  float b = 2.0 * (dot(rayDir, origin) -
                   ((1.0f + (k * k)) * (dot(rayDir, v) * dot(origin, v))));
  float c = (dot(origin, origin) -
             ((1.0f + (k * k)) * (dot(origin, v) * dot(origin, v))));

  float delta = b * b - 4.0f * a * c;

  if (delta < 0.0f || fabs(a) < 1e-6f)
    return false;

  float sqrtD = std::sqrt(delta);
  float inv2a = 0.5f / a;
  float t1 = (-b - sqrtD) * inv2a;
  float t2 = (-b + sqrtD) * inv2a;

  Vec3 p1 = ray.origin + ray.dir * t1;
  Vec3 p2 = ray.origin + ray.dir * t2;
  float m1 = dot(p1 - center, v);
  float m2 = dot(p2 - center, v);

  bool validT1 = (m1 > 0.0f);
  bool validT2 = (m2 > 0.0f);
  float t = std::numeric_limits<float>::infinity();
  float mHit = 0.0f;

  if (validT1 && t1 > ray.minHit && t1 < ray.maxHit) {
    t = t1;
    mHit = m1;
  }
  if (validT2 && t2 > ray.minHit && t2 < t) {
    t = t2;
    mHit = m2;
  }

  if (!std::isfinite(t))
    return false;

  hit.t = t;
  hit.position = ray.origin + ray.dir * t;
  Vec3 cp = hit.position - center;
  Vec3 normal_at_hit = normalize(cp - v * (k * mHit));
  hit.frontFace = dot(ray.dir, normal_at_hit) < 0;
  hit.normal = hit.frontFace ? normal_at_hit : -normal_at_hit;

  return true;
}
} // namespace shape
