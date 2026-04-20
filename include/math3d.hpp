#pragma once

#include <cmath>
#include <stdint.h>

#define EPS 0.0001f

class Ray;
class Hit;

class Vec3
{
public:
    float x;
    float y;
    float z;

    Vec3(float x_ = 0.f, float y_ = 0.f, float z_ = 0.f) : x(x_), y(y_), z(z_) {}

    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    Vec3 operator-(const Vec3 &other) const { return Vec3(x - other.x, y - other.y, z - other.z); }

    Vec3 operator+(const Vec3 &other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 &operator+=(const Vec3 &o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    Vec3 operator+(float s) const { return Vec3(x + s, y + s, z + s);}

    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    Vec3 operator*(const Vec3 &o) const { return Vec3(x * o.x, y * o.y, z * o.z); }
    Vec3 &operator*=(const Vec3 &o)
    {
        x *= o.x;
        y *= o.y;
        z *= o.z;
        return *this;
    }
    Vec3 &operator*=(float s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Vec3 operator/(float s) const { return Vec3(x / s, y / s, z / s); }
    Vec3 operator/(const Vec3 &other) const { 
        return Vec3(x / other.x, y / other.y, z / other.z); 
    }

    float &operator[](int i)
    {
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        else
            return z;
    }

    const float &operator[](int i) const
    {
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        return z;
    }

    inline float length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    inline float lengthSquared() const
    {
        return x * x + y * y + z * z;
    }
};

inline float dot(const Vec3 &a, const Vec3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline void display(const Vec3 &v)
{
    //std::cout << "X=" << v.x << ",Y=" << v.y << ",Z=" << v.z << std::endl;
}

inline Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
    return a * (1.0f - t) + b * t;
}

inline float norm(const Vec3 &v)
{
    return std::sqrt(dot(v, v));
}

inline Vec3 normalize(const Vec3 &v)
{
    const float eps = 1e-8f;
    float n = norm(v);

    if (n < eps)
        return Vec3(0.f, 0.f, 0.f);
    return v / n;
}

inline Vec3 reflect(const Vec3 &v, const Vec3 &n)
{
    return v - n * (2.0f * dot(v, n));
}

inline Vec3 cross(const Vec3 &a, const Vec3 &b)
{
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

inline Vec3 GetMax(Vec3 a, Vec3 b)
{
    Vec3 newMax;
    newMax.x = (a.x > b.x) ? a.x : b.x;
    newMax.y = (a.y > b.y) ? a.y : b.y;
    newMax.z = (a.z > b.z) ? a.z : b.z;
    return newMax;
}

inline Vec3 GetMin(Vec3 a, Vec3 b)
{
    Vec3 newMin;
    newMin.x = (a.x < b.x) ? a.x : b.x;
    newMin.y = (a.y < b.y) ? a.y : b.y;
    newMin.z = (a.z < b.z) ? a.z : b.z;
    return newMin;
}

class Vec2
{
public:
    float x;
    float y;

    Vec2(float x_ = 0.f, float y_ = 0.f) : x(x_), y(y_) {}
};


inline thread_local uint32_t rngState = 33554432;

inline const float SALT = 1.0f / 4294967296.0f;

inline float fastRandomFloat(float min, float max) noexcept {
    rngState ^= rngState << 13;
    rngState ^= rngState >> 17;
    rngState ^= rngState << 5;

    float randomNormalized = (float)rngState * SALT;
    
    return min + (max - min) * randomNormalized;
}
