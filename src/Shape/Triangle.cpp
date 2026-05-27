#include "Triangle.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

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

Triangle::Triangle(const libconfig::Setting &s) {
  _type = "Triangle";
  x = scene::readVec3(s["x"]);
  y = scene::readVec3(s["y"]);
  z = scene::readVec3(s["z"]);
  _color = scene::readVec3(s["color"]);
}

Triangle::Triangle(Vec3 x, Vec3 y, Vec3 z) {
  _type = "Triangle";
  this->x = x;
  this->y = y;
  this->z = z;
  _color = {255, 0, 255};
}

Triangle::Triangle(Vec3 x, Vec3 y, Vec3 z, Vec2 uvX, Vec2 uvY, Vec2 uvZ) {
  _type = "Triangle";
  this->x = x;
  this->y = y;
  this->z = z;
  this->uvX = uvX;
  this->uvY = uvY;
  this->uvZ = uvZ;
  _color = {255, 0, 255};
}

Vec2 Triangle::getUv(Vec3 &hitPos) const noexcept {
  Vec3 v0 = y - x;
  Vec3 v1 = z - x;
  Vec3 v2 = hitPos - x;

  float d00 = dot(v0, v0);
  float d01 = dot(v0, v1);
  float d11 = dot(v1, v1);
  float d20 = dot(v2, v0);
  float d21 = dot(v2, v1);

  float denom = d00 * d11 - d01 * d01;
  if (std::abs(denom) < 1e-8f)
    return {0.0f, 0.0f};

  float v = (d11 * d20 - d01 * d21) / denom;
  float w = (d00 * d21 - d01 * d20) / denom;
  float u = 1.0f - v - w;

  return {u * uvX.x + v * uvY.x + w * uvZ.x,
          u * uvX.y + v * uvY.y + w * uvZ.y};
}

AABB Triangle::getObjectAABB() const noexcept {
  Vec3 minP(std::min({x.x, y.x, z.x}), std::min({x.y, y.y, z.y}),
            std::min({x.z, y.z, z.z}));
  Vec3 maxP(std::max({x.x, y.x, z.x}), std::max({x.y, y.y, z.y}),
            std::max({x.z, y.z, z.z}));
  return {minP, maxP};
}

Vec3 Triangle::getCentroid() const noexcept { return (x + y + z) / 3.0f; }

bool Triangle::intersect(Ray &ray, Hit &hit) const noexcept {
  Vec3 edge1 = y - x;
  Vec3 edge2 = z - x;

  Vec3 pvec = cross(ray.dir, edge2);
  float det = dot(edge1, pvec);

  if (std::fabs(det) < EPS)
    return false;

  float invDet = 1.0f / det;
  Vec3 tvec = ray.origin - x;
  float u = dot(tvec, pvec) * invDet;

  if (u < 0.0f || u > 1.0f)
    return false;

  Vec3 qvec = cross(tvec, edge1);
  float v = dot(ray.dir, qvec) * invDet;
  if (v < 0.0f || u + v > 1.0f)
    return false;

  float t = dot(edge2, qvec) * invDet;

  if (t < ray.minHit || t > ray.maxHit)
    return false;
  hit.t = t;
  hit.position = ray.origin + ray.dir * t;

  Vec3 outwardNormal = normalize(cross(edge1, edge2));

  hit.frontFace = dot(ray.dir, outwardNormal) < 0;
  hit.normal = hit.frontFace ? outwardNormal : -outwardNormal;

  return true;
}

} // namespace shape