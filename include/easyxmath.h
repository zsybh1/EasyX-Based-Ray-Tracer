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

class pos3 {
public:
    float x, y, z;
    pos3(float X = 0, float Y = 0, float Z = 0) :x(X), y(Y), z(Z){}
    pos3 operator+(const vec3 &p) const { return pos3(x + p.x, y + p.y, z + p.z); }
    pos3 operator-(const vec3 &p) const { return pos3(x - p.x, y - p.y, z - p.z); }
    vec3 operator-(const pos3 &a) const { return vec3(x - a.x, y - a.y, z - a.z); }
};

class pos4 : public pos3 {
public:
    float w;
    pos4(float X = 0, float Y = 0, float Z = 0, float W = 1.0f) : pos3(X, Y, Z), w(W){}
    pos4(const pos3& p, float W = 1.0f) : pos3(p), w(W){}
    pos4(const vec3 &p, float W = 0.0f) : pos3(p.x, p.y, p.z), w(W){}
    pos4 operator+(const vec3 &p) const {
        return pos4(this->xyz() + p, w);
    }
    pos4 operator-(const vec3 &p) const {
        return pos4(this->xyz() - p, w);
    }
    vec3 operator-(const pos4 &a) const {
        return this->xyz() - a.xyz();
    }
    pos3 xyz() const {
        return pos3(x / w, y / w, z / w);
    }
};

class mat4 {
private:
    float *m_data;
public:
    mat4(float init = 1.0f) :m_data(new float[16]) {
        for (int i = 0; i < 16; ++i) {
            if (i % 5 == 0) {
                m_data[i] = init;
            }
            else {
                m_data[i] = 0.0f;
            }
        }
    }
    mat4(const mat4 &M) : m_data(new float[16]) {
        for (int i = 0; i < 16; ++i) {
            m_data[i] = M.m_data[i];
        }
    }
    mat4(mat4 &&M) : m_data(M.m_data) {
        M.m_data = nullptr;
    }

    mat4 &operator=(const mat4 &M) {
        for (int i = 0; i < 16; ++i) {
            m_data[i] = M.m_data[i];
        }
        return *this;
    }
    mat4 &operator=(mat4 &&M) {
        m_data = M.m_data;
        M.m_data = nullptr;
        return *this;
    }
    float *operator[](int i) const {
        return &m_data[4 * i];
    }
    mat4 operator*(const mat4 &N) const {
        mat4 P(0.0f);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    P[i][j] += (*this)[i][k] * N[k][j];
                }
            }
        }
        return P;
    }
    pos4 operator*(const pos4 &N) const {
        float res[4] = { 0.0f };
        for (int i = 0; i < 4; ++i) {
            res[i] = (*this)[i][0] * N.x + (*this)[i][1] * N.y + (*this)[i][2] * N.z + (*this)[i][3] * N.w;
        } 
        return pos4(res[0], res[1], res[2], res[3]);
    }
    ~mat4() {
        delete[] m_data;
    }
};

float radians(float degree) {
    return degree / 180.0f * acos(-1.0f);
}

mat4 translate_mat4(const vec3 &p) {
    mat4 M;
    M[0][0] = p.x;
    M[0][1] = p.y;
    M[0][2] = p.z;
    return M;
}

mat4 scale_mat4(const vec3 &p) {
    mat4 M;
    M[0][0] = p.x;
    M[1][1] = p.y;
    M[2][2] = p.z;
    return M;
}

//输入弧度
mat4 rotate_mat4(float rad, const vec3 &p) {
    mat4 M;
    vec3 q = p.normalize();
    float x = q.x;
    float y = q.y;
    float z = q.z;
    float c = cos(rad);
    float s = sin(rad);
    float _c = 1 - c;

    M[0][0] = c + x * x * _c;
    M[0][1] = x * y * _c - z * s;
    M[0][2] = x * z * _c + y * s;
    M[1][0] = y * z * _c + z * s;
    M[1][1] = c + y * y * _c;
    M[1][2] = y * z * _c - x * s;
    M[2][0] = z * x * _c - y * s;
    M[2][1] = z * y * _c + x * s;
    M[2][2] = c + z * z * _c;

    return M;
}

//rad是上下的视野角度，rate是屏幕宽高比，n是近裁剪平面，f是远裁剪平面
mat4 perspective_mat4(float rad, float rate, float n, float f) {
    float t = 1 / tan(rad / 2);
    mat4 M(0.0f);
    M[0][0] = t / rate;
    M[1][1] = t;
    M[2][2] = (f + n) / (n - f);
    M[2][3] = 2 * f * n / (f - n);
    M[3][2] = 1;

    return M;
}