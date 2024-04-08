#pragma once

#include <math.h>
#include <iostream>

template<typename T>
struct OLVector4
{
    T x, y, z, w;

    inline void operator=(const OLVector4<T>& a) { x = a.x; y = a.y; z = a.z; w = a.w; }
    inline void operator+=(const OLVector4<T>& a) { x += a.x; y += a.y; z += a.z; w += a.w; }
    inline void operator-=(const OLVector4<T>& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; }
    inline void operator*=(const OLVector4<T>& a) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; }
    inline void operator/=(const OLVector4<T>& a) { x /= a.x; y /= a.y; z /= a.z; w /= a.w; }
    inline void operator*=(const T f) { x *= f; y *= f; z *= f; w *= f; }
    inline void operator/=(const T f) { x /= f; y /= f; z /= f; w /= f; }
    inline OLVector4<T> operator -() { return OLVector4<T>{ -x, -y, -z, -w }; }
};

template<typename T>
inline OLVector4<T> operator+(const OLVector4<T>& a, const OLVector4<T>& b) { return OLVector4<T>{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
template<typename T>
inline OLVector4<T> operator-(const OLVector4<T>& a, const OLVector4<T>& b) { return OLVector4<T>{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }
template<typename T>
inline OLVector4<T> operator*(const OLVector4<T>& a, const OLVector4<T>& b) { return OLVector4<T>{ a.x* b.x, a.y* b.y, a.z* b.z, a.w* b.w }; }
template<typename T>
inline OLVector4<T> operator/(const OLVector4<T>& a, const OLVector4<T>& b) { return OLVector4<T>{ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }; }
template<typename T>
inline OLVector4<T> operator*(const OLVector4<T>& a, const T f) { return OLVector4<T>{ a.x* f, a.y* f, a.z* f, a.w* f }; }
template<typename T>
inline OLVector4<T> operator/(const OLVector4<T>& a, const T f) { return OLVector4<T>{ a.x / f, a.y / f, a.z / f, a.w / f }; }
template<typename T>
inline bool operator==(const OLVector4<T>& a, const OLVector4<T>& b) { return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w); }

template<typename T>
inline T operator^(const OLVector4<T>& a, const OLVector4<T>& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w); }
template<typename T>
inline T mag_sq(const OLVector4<T>& a) { return (a.x * a.x) + (a.y * a.y) + (a.z * a.z) + (a.w * a.w); }
template<typename T>
inline float mag(const OLVector4<T>& a) { return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z) + (a.w * a.w)); }
template<typename T>
inline OLVector4<T> norm(const OLVector4<T>& a) { return a / mag(a); }
template<typename T>
inline OLVector4<T> lerp(const OLVector4<T>& a, const OLVector4<T>& b, const T f) { OLVector4<T>{ a.x + ((b.x - a.x) * f), a.y + ((b.y - a.y) * f), a.z + ((b.z - a.z) * f), a.w + ((b.w - a.w) * f) }; }

template<typename T>
inline std::ostream& operator<<(std::ostream& stream, const OLVector4<T>& v) { return stream << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')'; }

typedef OLVector4<float> OLVector4f;
typedef OLVector4<int32_t> OLVector4i;
typedef OLVector4<uint32_t> OLVector4u;