#include "Chrome.hpp"

Chrome::Chrome(const libconfig::Setting& s)
{
    this->type = "Chrome";
    this->color = {255, 255, 255};
    this->metallic = 1.0f;
    this->roughness = 0.05f;
    this->ior = 1.0f;
}

Chrome::Chrome()
{
    this->type = "Chrome";
    this->color = {255, 255, 255};
    this->metallic = 1.0f;
    this->roughness = 0.05f;
    this->ior = 1.0f;
}