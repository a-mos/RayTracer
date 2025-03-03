#pragma once
#include <cmath>
#include <vector>
#include <iostream>

template<size_t DIM, typename T>
struct vec {
    vec() { for (size_t i = DIM; i--; data_[i] = T()); }

    T &operator[](const size_t i) {
        return data_[i];
    }

    const T &operator[](const size_t i) const {
        return data_[i];
    }

private:
    T data_[DIM];
};

typedef vec<2, float> Vec2f;
typedef vec<3, float> Vec3f;
typedef vec<3, double > Vec3d;
typedef vec<3, int> Vec3i;
typedef vec<4, float> Vec4f;


template<typename T>
struct vec<3, T> {
    vec() : x(T()), y(T()), z(T()) {}

    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    T &operator[](const size_t i) {
        return i <= 0 ? x : (1 == i ? y : z);
    }

    const T &operator[](const size_t i) const {
        return i <= 0 ? x : (1 == i ? y : z);
    }

    float norm() { return std::sqrt(x * x + y * y + z * z); }

    vec<3, T> &normalize(T l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }
    friend vec<3, T> operator/(const T &v1, const vec<3, T> &v2) {
        return vec<3, T>(v1 / v2.x, v1 / v2.y, v1 / v2.z);
    }

    T x, y, z;
};

template<typename T>
struct vec<4, T> {
    vec() : x(T()), y(T()), z(T()), w(T()) {}

    vec(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}

    T &operator[](const size_t i) {
        return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w));
    }

    const T &operator[](const size_t i) const {
        return i <= 0 ? x : (1 == i ? y : (2 == i ? z : w));
    }

    T x, y, z, w;
};

template<size_t DIM, typename T>
T operator*(const vec<DIM, T> &lhs, const vec<DIM, T> &rhs) {
    T ret = T();
    for (size_t i = DIM; i--; ret += lhs[i] * rhs[i]);
    return ret;
}

template<size_t DIM, typename T>
vec<DIM, T> operator+(vec<DIM, T> lhs, const vec<DIM, T> &rhs) {
    for (size_t i = DIM; i--; lhs[i] += rhs[i]);
    return lhs;
}

template<size_t DIM, typename T>
vec<DIM, T> operator-(vec<DIM, T> lhs, const vec<DIM, T> &rhs) {
    for (size_t i = DIM; i--; lhs[i] -= rhs[i]);
    return lhs;
}

template<size_t DIM, typename T, typename U>
vec<DIM, T> operator*(const vec<DIM, T> &lhs, const U &rhs) {
    vec<DIM, T> ret;
    for (size_t i = DIM; i--; ret[i] = lhs[i] * rhs);
    return ret;
}

template<size_t DIM, typename T>
vec<DIM, T> operator-(const vec<DIM, T> &lhs) {
    return lhs * T(-1);
}

template<typename T>
vec<3, T> cross(const vec<3, T> &v1, const vec<3, T> &v2) {
    return vec<3, T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

template<typename T>
float clamp(float& v, T lo, T hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

template<typename T>
float clamp(T v, const T lo, const T hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

template<typename T>
vec<3, T> mul(const vec<3, T> &v1, const vec<3, T> &v2) {
    return vec<3, T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}