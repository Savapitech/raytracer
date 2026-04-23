#include "Mandelbulb.hpp"

Mandelbulb::Mandelbulb(const libconfig::Setting &s)
{
    _type = "Mandelbulb";
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
    Vec3 z = p;
    float dr = 1.0f;
    float r = 0.0f;
    for (int i = 0; i < _iterations; i++) {
        r = norm(z);
        if (r > 2.0f)
            break;
        float theta = std::acos(z.z / r);
        float phi = std::atan2(z.y, z.x);
        dr = std::pow(r, _power - 1.0) * _power * dr + 1.0;
        float zr = std::pow(r, _power);
        theta = theta * _power;
        phi = phi * _power;
        Vec3 vec(std::sin(theta) * std::cos(phi), std::sin(phi) * std::sin(theta), std::cos(theta));
        z = vec * zr;
        z += p;
    }
    return 0.5 * std::log(r) * r / dr;
}
