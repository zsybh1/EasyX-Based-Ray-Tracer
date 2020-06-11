#pragma once
#include <cmath>
class vec3 {
public:
    float x, y, z;
    vec3(float X, float Y, float Z = 0) {
        x = X;
        y = Y;
        z = Z;
    }
    vec3(float X = 0) { x = y = z = X; }
    vec3 operator+(const vec3 &p) const{ return vec3(x + p.x, y + p.y, z + p.z); }
    vec3 operator-(const vec3 &p) const{ return vec3(x - p.x, y - p.y, z - p.z); }
    vec3 operator-() const { return vec3(-x, -y, -z); }
    vec3 operator*(float scale) const {
        return vec3(scale * x, scale * y, scale * z);
    }
    friend vec3 operator*(float scale, const vec3 &p) {
        return vec3(scale * p.x, scale * p.y, scale * p.z);
    }
    friend float dot(const vec3 &p, const vec3 &q) {
        return p.x * q.x + p.y * q.y + p.z * q.z;
    }
    friend vec3 cross(const vec3 &p, const vec3 &q) {
        return vec3(p.y * q.z - p.z * q.y, p.z * q.x - p.x * q.z, p.x * q.y - q.x * p.y);
    }
    float length() const{
        return sqrt(dot(*this, *this));
    }
    vec3 normalize() const {
        return *this * (1.0f / length());
    }
};

class pos {
public:
    float x, y, z;
    pos(float X = 0, float Y = 0, float Z = 0) {
        x = X;
        y = Y;
        z = Z;
    }
    pos operator+(const vec3 &p) const { return pos(x + p.x, y + p.y, z + p.z); }
    pos operator-(const vec3 &p) const { return pos(x - p.x, y - p.y, z - p.z); }
    vec3 operator-(const pos &a) const { return vec3(x - a.x, y - a.y, z - a.z); }
};
