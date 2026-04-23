#include "Mandelbulb.hpp"

Mandelbulb::Mandelbulb(const libconfig &s)
{
    _type = "sphereMarching";
    _power = (float)s["power"];
    _pos = scene::readVec3(s["pos"]);
    _color = scene::readVec3(s["color"]);
    _iterations = (int)s["iterations"];
}

AABB Mandelbulb::getObjectAABB() const
{
    float size = 1.5f;

    return {{_pos.x - size, _pos.y - size, _pos.z - size},{_pos.x + size, _pos.y + size, _pos.z + size}};
}

Vec3 Mandelbulb::getCentroid() const
{
    return _pos;
}

float Mandelbulb::evaluateSDF(const Vec3 &p) const
{

}