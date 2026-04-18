#include <libconfig.h++>

#include "Object.hpp"

Object::Object(const libconfig::Setting &s)
{
    if (!s["shape"].exists("type"))
        throw std::runtime_error("Type need to be implemented in a shape");

    std::string type = s["shape"]["type"];

    this->_shape = getObjectFactory().shapeFactory.getShape(s["shape"]);
    if (this->_shape == nullptr)
        throw std::invalid_argument("Need type to implement shape");

    this->_material = getObjectFactory().materialFactory.getMaterial(s);
    this->_aabb = this->_shape->getObjectAABB();
    this->_centroid = this->_shape->getCentroid();
}

Object::Object(std::unique_ptr<IShape> shape, std::unique_ptr<AMaterial> material)
{
    this->_shape = std::move(shape);
    this->_material = std::move(material);
    this->_aabb = this->_shape->getObjectAABB();
    this->_centroid = this->_shape->getCentroid();
}
