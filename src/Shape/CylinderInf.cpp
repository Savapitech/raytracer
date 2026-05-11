#include "CylinderInf.hpp"

#include "Object.hpp"
#include "Ray.hpp"

CylinderInf::CylinderInf(const libconfig::Setting &s) {
  _type = "CylinderInf";
  _pos = scene::readVec3(s["pos"]);
  _heigth = (float)s["height"];
  _radius = (float)s["radius"];
  _color = scene::readVec3(s["color"]);
  if (s.exists("dir"))
    _dir = scene::readVec3(s["dir"]);
  else
    _dir = {1, 0, 0};
}

AABB CylinderInf::getObjectAABB() const noexcept {
  float inf = std::numeric_limits<float>::infinity();
  return AABB(Vec3(-inf, -inf, -inf), Vec3(inf, inf, inf));
}

Vec3 CylinderInf::getCentroid() const noexcept { return _pos; }

Vec2 CylinderInf::getUv(Vec3 &) const noexcept { return {0, 0}; }

bool CylinderInf::intersect(Ray &ray, Hit &hit) const noexcept {
  Vec3 center = _pos;
  float r = _radius;
  Vec3 v = normalize(_dir);
  Vec3 origin = ray.origin - this->_pos;
  Vec3 ray_dir = ray.dir;

  float a = (dot(ray_dir, ray_dir)) - ((dot(ray_dir, v)) * (dot(ray_dir, v)));
  float b =
      2.0 * ((dot(ray_dir, origin)) - (dot(ray_dir, v)) * (dot(origin, v)));
  float c = (dot(origin, origin)) - (dot(origin, v) * dot(origin, v)) - (r * r);

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

  float t = std::numeric_limits<float>::infinity();
  Vec3 normal_at_hit;

  if (t1 > ray.minHit && t1 < ray.maxHit) {
    t = t1;
    normal_at_hit =
        normalize((ray.origin + ray.dir * t) - this->_pos - (v * m1));
  }
  if (t2 > ray.minHit && t2 < t) {
    t = t2;
    normal_at_hit =
        normalize((ray.origin + ray.dir * t) - this->_pos - (v * m2));
  }

  if (!std::isfinite(t))
    return false;

  hit.t = t;
  hit.position = ray.origin + ray.dir * t;
  hit.frontFace = dot(ray.dir, normal_at_hit) < 0;
  hit.normal = hit.frontFace ? normal_at_hit : -normal_at_hit;

  return true;
}