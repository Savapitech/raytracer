#pragma once
#include "AFractals.hpp"
#include <cmath>    
#include <algorithm>

class MobiusStrip : public AFractals
{
    public:
        MobiusStrip(const libconfig::Setting &s);
        Vec3 getCentroid() const override;
        float evaluateSDF(const Vec3 &p) const override;
        AABB getObjectAABB() const override;
    private:
        float _stripRadius;
        float _thickness;
        float _width;
};
