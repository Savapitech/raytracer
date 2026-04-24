#include "IMaterials.hpp"

AMaterial::AMaterial(const libconfig::Setting& s)
{
    this->type = "Glass";
    this->color = {255, 255, 255};
    this->metallic = 0.0f;
    this->roughness = 0.0f;
    this->ior = 1.5f;
    this->transmission = 1.0f;
}