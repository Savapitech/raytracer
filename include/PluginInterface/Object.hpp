#include <iostream>
#include <memory>
#include <libconfig.h++>

#include "bvh.hpp"
#include "logger.hpp"
#include "math3d.hpp"

class IShape
{
    public:
        virtual ~IShape() = default;
        //virtual bool intersect(Ray&, Hit&) const = 0;
        //virtual AABB getObjectAABB() const = 0;
        //virtual const Vec3& getPos(void) const = 0;
};

class AShape : public IShape
{
    public:
        void ShowShape(void){Log::Logger::debug("Object Shape:" + type);}
        std::string type;
};

class Sphere final : public AShape  
{
public:
    float radius;
    Vec3 pos;

    Sphere(const Vec3& c, float r) {
        type = "Sphere";
        radius = r;
        pos = c;
    }
};

class ShapeFactory{
    public:
        ShapeFactory();
        std::unique_ptr<AShape> GetShape(const libconfig::Setting &s);
};

class ObjectFactory{
        public:
            /*std::unique_ptr<IMaterial> GetShape(const Setting &s);*/
            ShapeFactory SFactory;
};

class Object
{
    public:
        Object(const libconfig::Setting &s, bool materialExist);
        AABB aabb;
        std::unique_ptr<AShape> shape;
    private:
        static ObjectFactory OFactory;
};
