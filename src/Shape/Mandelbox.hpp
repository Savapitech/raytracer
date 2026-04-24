#pragma once
#include "AFractals.hpp"
#include <cmath>
#include <algorithm>

class Mandelbox : public AFractals {
    public:
        Mandelbox(const libconfig::Setting &s);
        float evaluateSDF(const Vec3 &p) const override;
        AABB getObjectAABB() const override;
        Vec3 getCentroid() const override;
        
    private:
        float _scale;
        int _iterations;
        bool _juliaMode;
        Vec3 _juliaConstant;
};
