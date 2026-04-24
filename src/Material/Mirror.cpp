#include "Mirror.hpp"
#include "Ray.hpp"

Mirror::Mirror(const libconfig::Setting&)
{
    this->type = "Mirror";
    this->color = {255, 255, 255};
    this->metallic = 1.0f;
    this->roughness = 0.05f;
    this->ior = 1.0f;
}
