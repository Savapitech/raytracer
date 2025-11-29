#pragma once

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