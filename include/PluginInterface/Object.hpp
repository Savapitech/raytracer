#include <iostream>
#include <memory>
struct Vec3
{
    double x;
    double y;
    double z;
};

struct AABB
{
    Vec3 max;
    Vec3 min;
};

struct Ray
{
    /* data */
};

struct Hit
{
    /* data */
};

class IShape
{
    public:
        virtual ~IShape() = default;
        virtual bool intersect(Ray&, Hit&) const = 0;
        virtual AABB getObjectAABB() const = 0;
        virtual const Vec3& getPos(void) const = 0;
};

class AShape : IShape
{
    public:
        void ShowShape(void);
    protected:
        std::string type;
        
};

class Sphere final : AShape  
{
    float radius;
    Vec3 pos;
    Sphere(const Vec3& c, float r) : radius(r), pos(c) {}
    
    const Vec3& getPos(void) const override;
    bool intersect(Ray& ray, Hit& hit) const override;
    AABB getObjectAABB() const override;
};

class Object
{
    public:
        AABB aabb;
        std::unique_ptr<IShape> shape;
};
