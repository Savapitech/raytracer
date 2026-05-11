#include "AFractals.hpp"

bool AFractals::intersect(Ray &ray, Hit &hit) const {
  float t = ray.minHit;

  for (int i = 0; i < _maxIt; i++) {
    Vec3 currentPosition = ray.origin + ray.dir * t;
    Vec3 localPosition = currentPosition - _pos;
    float dist = evaluateSDF(localPosition);
    if (dist < _collisionEpsilon) {
      hit.t = t;
      hit.position = currentPosition;
      hit.normal = calculateNormale(localPosition);
      hit.frontFace = (dot(ray.dir, hit.normal) < 0);
      if (hit.frontFace == true)
        hit.normal = hit.normal;
      else
        hit.normal = hit.normal * -1.0f;
      return true;
    }
    t += dist;
    if (t >= ray.maxHit || t >= _escapeDist)
      break;
  }
  return false;
}

Vec2 AFractals::getUv(Vec3 &) const { return {0, 0}; }

Vec3 AFractals::calculateNormale(const Vec3 &p) const {
  const float epsilon = 0.0001f;

  float dx = evaluateSDF(Vec3(p.x + epsilon, p.y, p.z)) -
             evaluateSDF(Vec3(p.x - epsilon, p.y, p.z));
  float dy = evaluateSDF(Vec3(p.x, p.y + epsilon, p.z)) -
             evaluateSDF(Vec3(p.x, p.y - epsilon, p.z));
  float dz = evaluateSDF(Vec3(p.x, p.y, p.z + epsilon)) -
             evaluateSDF(Vec3(p.x, p.y, p.z - epsilon));

  Vec3 vec(dx, dy, dz);
  return normalize(vec);
}
