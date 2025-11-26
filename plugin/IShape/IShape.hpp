#pragma once

#include <iostream>
#include <functional>
#include <any>
#include <memory>



class IShape {
    public:
        virtual ~IShape() = default;
        virtual std::string getName() const = 0;
    private:
        std::string type;
};


class AShape : public IShape {
    public:
        AShape(const std::string &type) : name(type) {}
        virtual ~AShape() = default;
        std::string getName() const override { return name; }
    private:
        std::string name;
};

class Sphere final: public AShape {
public:
    Sphere() : AShape("Sphere") {}
};

class Cube final: public AShape {
public:
    Cube() : AShape("Cube") {}
};

std::unique_ptr<IShape> createObject(const char* type) {
    std::string t(type);
    if (t == "sphere") 
        return std::make_unique<Sphere>();
    if (t == "cube") 
        return std::make_unique<Cube>();
    return nullptr;
}


struct IPlugin
{
    std::function<std::unique_ptr<IShape>(const char*)> create;
};

static IPlugin desc;

extern "C" IPlugin *getPluginDescriptor() {
    desc.create = createObject;
    return &desc;
}
