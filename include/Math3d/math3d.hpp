#pragma once

#include <cmath>
#include <iostream>

struct Vec3 {
    float x, y, z;
    Vec3(float x_=0.f, float y_=0.f, float z_=0.f) : x(x_), y(y_), z(z_) {}

    Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }
    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    Vec3 operator*(float s) const { return Vec3(x*s, y*s, z*s); }
    Vec3 operator/(float s) const { return Vec3(x/s, y/s, z/s); }
};

inline float dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline void display(const Vec3& v) {
    std::cout << "X=" << v.x << ",Y=" << v.y << ",Z=" << v.z << std::endl;
}

inline float norm(const Vec3& v) {
    return std::sqrt(dot(v, v));
}

inline Vec3 normalize(const Vec3& v) {
    const float EPS = 1e-8f;
    float n = norm(v);

    if (n < EPS) return Vec3(0.f, 0.f, 0.f);
        return v / n;
}

struct AABB
{
    Vec3 max;
    Vec3 min;
};