#include "RectangleXZ.hpp"
#include "Object.hpp"
#include "Ray.hpp"

RectangleXZ::RectangleXZ(const libconfig::Setting &s) {
  _type = "RectangleXZ";
  x = scene::readVec2(s["x"]);
  z = scene::readVec2(s["z"]);
  y = (float)(s["y"]);
  _color = scene::readVec3(s["color"]);
}

AABB RectangleXZ::getObjectAABB() const noexcept {
  return AABB(Vec3(x.x, y - 0.01f, z.x), Vec3(x.y, y + 0.01f, z.y));
}

Vec3 RectangleXZ::getCentroid() const noexcept {
  return Vec3((this->x.x + this->x.y) / 2, this->y,
              (this->z.x + this->z.y) / 2);
}

Vec2 RectangleXZ::getUv(Vec3 &) const noexcept { return {0, 0}; }

bool RectangleXZ::intersect(Ray &ray, Hit &hit) const noexcept {
  if (std::abs(ray.dir.y) < 1e-6f)
    return false;

  float t = (y - ray.origin.y) / ray.dir.y;

  if (t < ray.minHit || t > ray.maxHit)
    return false;

  float hitX = ray.origin.x + t * ray.dir.x;
  float hitZ = ray.origin.z + t * ray.dir.z;

  float minX = std::min(this->x.x, this->x.y);
  float maxX = std::max(this->x.x, this->x.y);
  float minZ = std::min(this->z.x, this->z.y);
  float maxZ = std::max(this->z.x, this->z.y);

  if (hitX < minX || hitX > maxX || hitZ < minZ || hitZ > maxZ)
    return false;

  hit.t = t;
  hit.position = ray.origin + ray.dir * t;
  Vec3 outward_normal(0, 1, 0);

  if (dot(ray.dir, outward_normal) < 0.0f) {
    hit.frontFace = true;
    hit.normal = outward_normal;
  } else {
    hit.frontFace = false;
    hit.normal = outward_normal * -1.0f;
  }

  return true;
}