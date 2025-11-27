#include <iostream>

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
        virtual bool intersect(const Ray&, Hit&) const = 0;
        virtual AABB getObjectAABB() const = 0;
    private:
        std::string type;
};

class AShape : IShape
{
    public:
        void ShowShape(void);
};


class Object
{
    public:
    private:
};
