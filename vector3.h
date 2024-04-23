#pragma once

#include <math.h>
#include <iostream>

#define OL_RIGHT { 1,0,0 }
#define OL_UP { 0,1,0 }
#define OL_BACK { 0,0,1 }
#define OL_ONE { 1,1,1 }
#define OL_ZERO { 0,0,0 }

template <typename T> struct OLVector3;

template<typename T>
struct OLVector3
{
    T x, y, z;

    inline void operator=(const OLVector3<T>& a) { x = a.x; y = a.y; z = a.z; }
    inline void operator+=(const OLVector3<T>& a) { x += a.x; y += a.y; z += a.z; }
    inline void operator-=(const OLVector3<T>& a) { x -= a.x; y -= a.y; z -= a.z; }
    inline void operator*=(const OLVector3<T>& a) { x *= a.x; y *= a.y; z *= a.z; }
    inline void operator/=(const OLVector3<T>& a) { x /= a.x; y /= a.y; z /= a.z; }
    inline void operator*=(const T f) { x *= f; y *= f; z *= f; }
    inline void operator/=(const T f) { x /= f; y /= f; z /= f; }
    inline OLVector3<T> operator -() { return OLVector3<T>{ -x, -y, -z }; }
};

template<typename T>
inline OLVector3<T> operator+(const OLVector3<T>& a, const OLVector3<T>& b) { return OLVector3<T>{ a.x + b.x, a.y + b.y, a.z + b.z }; }
template<typename T>
inline OLVector3<T> operator-(const OLVector3<T>& a, const OLVector3<T>& b) { return OLVector3<T>{ a.x - b.x, a.y - b.y, a.z - b.z }; }
template<typename T>
inline OLVector3<T> operator*(const OLVector3<T>& a, const OLVector3<T>& b) { return OLVector3<T>{ a.x * b.x, a.y * b.y, a.z * b.z }; }
template<typename T>
inline OLVector3<T> operator/(const OLVector3<T>& a, const OLVector3<T>& b) { return OLVector3<T>{ a.x / b.x, a.y / b.y, a.z / b.z }; }
template<typename T>
inline OLVector3<T> operator*(const OLVector3<T>& a, const float f) { return OLVector3<T>{ a.x * f, a.y * f, a.z * f }; }
template<typename T>
inline OLVector3<T> operator/(const OLVector3<T>& a, const float f) { return OLVector3<T>{ a.x / f, a.y / f, a.z / f }; }
template<typename T>
inline bool operator==(const OLVector3<T>& a, const OLVector3<T>& b) { return (a.x == b.x) && (a.y == b.y) && (a.z == b.z); }

template<typename T>
inline T operator^(const OLVector3<T>& a, const OLVector3<T>& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
template<typename T>
inline OLVector3<T> operator%(const OLVector3<T>& a, const OLVector3<T>& b) { return OLVector3<T>{ (a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x) }; }

template<typename T>
inline T sq_mag(const OLVector3<T>& a) { return a ^ a; }
template<typename T>
inline float mag(const OLVector3<T>& a) { return sqrt(sq_mag(a)); }
template<typename T>
inline OLVector3<T> norm(const OLVector3<T>& a) { return a / mag(a); }
template<typename T>
inline OLVector3<T> lerp(const OLVector3<T>& a, const OLVector3<T>& b, const T f) { OLVector3<T>{ a.x + ((b.x - a.x) * f), a.y + ((b.y - a.y) * f), a.z + ((b.z - a.z) * f) }; }

template<typename T>
inline std::ostream& operator<<(std::ostream& stream, const OLVector3<T>& v) { return stream << '(' << v.x << ", " << v.y << ", " << v.z << ')'; }

typedef OLVector3<float> OLVector3f;
typedef OLVector3<int32_t> OLVector3i;
typedef OLVector3<uint32_t> OLVector3u;