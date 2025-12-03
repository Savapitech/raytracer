#pragma once

#include <cmath>
#include <iostream>

class Ray;
class Hit;

class Vec3 {
    public:
    float x, y, z;
    Vec3(float x_=0.f, float y_=0.f, float z_=0.f) : x(x_), y(y_), z(z_) {}

    Vec3 operator-() const {return Vec3(-x, -y, -z);}
    Vec3 operator-(const Vec3& other) const {return Vec3(x - other.x, y - other.y, z - other.z);}

    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z);}
    Vec3& operator+=(const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this;}

    Vec3 operator*(float s) const { return Vec3(x*s, y*s, z*s);}
    Vec3 operator*(const Vec3& o) const { return Vec3(x * o.x, y * o.y, z * o.z);}
    Vec3& operator*=(const Vec3& o) { x *= o.x; y *= o.y; z *= o.z; return *this;}
    Vec3& operator*=(float s) { x *= s; y *= s; z *= s; return *this;}


    Vec3 operator/(float s) const { return Vec3(x/s, y/s, z/s);}

    float& operator[](int i) {
    if (i == 0) return x;
    if (i == 1) return y;
    else return z;
    }

    const float& operator[](int i) const {
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
    }

    inline float length() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    inline float lengthSquared() const {
        return x*x + y*y + z*z;
    }
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

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - n * (2.0f * dot(v, n));
}



class AABB
{
    public:
        Vec3 min;
        Vec3 max;
        float surfaceArea() const {
            Vec3 d = max - min;

            if (d.x < 0 || d.y < 0 || d.z < 0)
                return 0.0f;
            return 2.0f * (d.x * d.y + d.y * d.z + d.z * d.x);
            }
        bool intersect(const Ray& r) const;
        void normalize();
};