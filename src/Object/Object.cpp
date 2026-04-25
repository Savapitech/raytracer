#include <libconfig.h++>

#include "Object.hpp"

Object::Object(const libconfig::Setting &s)
{
    if (!s["shape"].exists("type"))
        throw std::runtime_error("Type need to be implemented in a shape");

    std::string type = s["shape"]["type"];

    _shape = getObjectFactory().shapeFactory.getShape(s["shape"]);
    if (_shape == nullptr)
        throw std::invalid_argument("Need type to implement shape");

    _material = getObjectFactory().materialFactory.getMaterial(s);
    _aabb = _shape->getObjectAABB();
    _centroid = _shape->getCentroid();
}

Object::Object(std::unique_ptr<IShape> shape, std::unique_ptr<AMaterial> material)
{
    _shape = std::move(shape);
    _material = std::move(material);
    _shape->getColor();
    _aabb = _shape->getObjectAABB();
    _centroid = _shape->getCentroid();
}
