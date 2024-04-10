#pragma once

#include <math.h>
#include <iostream>

template<typename T>
struct OLVector2
{
	T x, y;

	inline void operator=(const OLVector2<T>& a) { x = a.x; y = a.y; }
	inline void operator+=(const OLVector2<T>& a) { x += a.x; y += a.y; }
	inline void operator-=(const OLVector2<T>& a) { x -= a.x; y -= a.y; }
	inline void operator*=(const OLVector2<T>& a) { x *= a.x; y *= a.y; }
	inline void operator/=(const OLVector2<T>& a) { x /= a.x; y /= a.y; }
	inline void operator*=(const T f) { x *= f; y *= f; }
	inline void operator/=(const T f) { x /= f; y /= f; }
	inline OLVector2<T> operator -() { return OLVector2<T>{ -x, -y }; }
};

template<typename T>
inline OLVector2<T> operator+(const OLVector2<T>& a, const OLVector2<T>& b) { return OLVector2<T>{ a.x + b.x, a.y + b.y }; }
template<typename T>
inline OLVector2<T> operator-(const OLVector2<T>& a, const OLVector2<T>& b) { return OLVector2<T>{ a.x - b.x, a.y - b.y }; }
template<typename T>
inline OLVector2<T> operator*(const OLVector2<T>& a, const OLVector2<T>& b) { return OLVector2<T>{ a.x * b.x, a.y * b.y }; }
template<typename T>
inline OLVector2<T> operator/(const OLVector2<T>& a, const OLVector2<T>& b) { return OLVector2<T>{ a.x / b.x, a.y / b.y }; }
template<typename T>
inline OLVector2<T> operator*(const OLVector2<T>& a, const T f) { return OLVector2<T>{ a.x * f, a.y * f }; }
template<typename T>
inline OLVector2<T> operator/(const OLVector2<T>& a, const T f) { return OLVector2<T>{ a.x / f, a.y / f }; }
template<typename T>
inline bool operator==(const OLVector2<T>& a, const OLVector2<T>& b) { return (a.x == b.x) && (a.y == b.y); }

template<typename T>
inline T operator^(const OLVector2<T>& a, const OLVector2<T>& b) { return (a.x * b.x) + (a.y * b.y); }

template<typename T>
inline T sq_mag(const OLVector2<T>& a) { return a ^ a; }
template<typename T>
inline float mag(const OLVector2<T>& a) { return sqrt(sq_mag(a)); }
template<typename T>
inline OLVector2<T> norm(const OLVector2<T>& a) { return a / mag(a); }

template<typename T>
inline std::ostream& operator<<(std::ostream& stream, const OLVector2<T>& v) { return stream << '(' << v.x << ", " << v.y << ')'; }

typedef OLVector2<float> OLVector2f;
typedef OLVector2<int32_t> OLVector2i;
typedef OLVector2<uint32_t> OLVector2u;