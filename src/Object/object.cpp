#include <libconfig.h++>
#include <memory>
#include <map>
#include <functional>

#include "Scene.hpp"

ObjectFactory OFactory;

Object::Object(const libconfig::Setting &s)
{
    if (!s["shape"].exists("type"))
        throw std::runtime_error("Type need to be implemented in a shape");
    std::string type = s["shape"]["type"];
    this->shape = OFactory.SFactory.GetShape(s["shape"]);
    if (this->shape == nullptr)
        throw std::invalid_argument("Need type to implement shape");
    this->material = OFactory.MFactory.GetMaterial(s);
    this->aabb = this->shape->getObjectAABB();
    this->centroid = this->shape->getCentroid();
}

Object::Object(std::unique_ptr<AShape> shape, std::unique_ptr<AMaterial> material)
{
    this->shape = std::move(shape);
    this->material = std::move(material);
    this->aabb = this->shape->getObjectAABB();
    this->centroid = this->shape->getCentroid();
}
