#include "common.h"

HINATA_NAMESPACE_BEGIN

template <typename T>
HINATA_FORCE_INLINE Vec2<T>::Vec2()
	: x(T(0))
	, y(T(0))
{

}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>::Vec2(const Vec2<T>& v)
	: x(v.x)
	, y(v.y)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>::Vec2(const Vec3<T>& v)
	: x(v.x)
	, y(v.y)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>::Vec2(const Vec4<T>& v)
	: x(v.x)
	, y(v.y)
{

}

template <typename T>
template <typename T2>
HINATA_FORCE_INLINE Vec2<T>::Vec2(const Vec2<T2>& v)
	: x(T(v.x))
	, y(T(v.y))
{

}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>::Vec2(T v)
	: x(v)
	, y(v)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>::Vec2(T x, T y)
	: x(x)
	, y(y)
{

}

template <typename T>
HINATA_FORCE_INLINE T& Vec2<T>::operator[](int i)
{
	return (&x)[i];
}

template <typename T>
HINATA_FORCE_INLINE const T& Vec2<T>::operator[](int i) const
{
	return (&x)[i];
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>& Vec2<T>::operator=(const Vec2<T>& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>& Vec2<T>::operator*=(const Vec2<T>& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>& Vec2<T>::operator/=(const Vec2<T>& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>& Vec2<T>::operator++()
{
	++x;
	++y;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> Vec2<T>::operator++(int)
{
	return Vec2<T>(x + T(1), y + T(1));
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T>& Vec2<T>::operator--()
{
	--x;
	--y;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> Vec2<T>::operator--(int)
{
	return Vec2<T>(x - T(1), y - T(1));
}

// Binary + operator
template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator+(const Vec2<T>& v, T s)
{
	return Vec2<T>(v.x + s, v.y + s);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator+(T s, const Vec2<T>& v)
{
	return Vec2<T>(s + v.x, s + v.y);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator+(const Vec2<T>& v1, const Vec2<T>& v2)
{
	return Vec2<T>(v1.x + v2.x, v1.y + v2.y);
}

// Binary - operator
template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator-(const Vec2<T>& v, T s)
{
	return Vec2<T>(v.x - s, v.y - s);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator-(T s, const Vec2<T>& v)
{
	return Vec2<T>(s - v.x, s - v.y);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator-(const Vec2<T>& v1, const Vec2<T>& v2)
{
	return Vec2<T>(v1.x - v2.x, v1.y - v2.y);
}

// Binary * operator
template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator*(const Vec2<T>& v, T s)
{
	return Vec2<T>(v.x * s, v.y * s);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator*(T s, const Vec2<T>& v)
{
	return Vec2<T>(s * v.x, s * v.y);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator*(const Vec2<T>& v1, const Vec2<T>& v2)
{
	return Vec2<T>(v1.x * v2.x, v1.y * v2.y);
}

// Binary / operator
template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator/(const Vec2<T>& v, T s)
{
	return Vec2<T>(v.x / s, v.y / s);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator/(T s, const Vec2<T>& v)
{
	return Vec2<T>(s / v.x, s / v.y);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator/(const Vec2<T>& v1, const Vec2<T>& v2)
{
	return Vec2<T>(v1.x / v2.x, v1.y / v2.y);
}

// Unary - operator
template <typename T>
HINATA_FORCE_INLINE Vec2<T> operator-(const Vec2<T>& v)
{
	return Vec2<T>(-v.x, -v.y);
}

// Boolean operators
template <typename T>
HINATA_FORCE_INLINE bool operator==(const Vec2<T>& v1, const Vec2<T>& v2)
{
	return v1.x == v2.x && v1.y == v2.y;
}

template <typename T>
HINATA_FORCE_INLINE bool operator!=(const Vec2<T>& v1, const Vec2<T>& v2)
{
	return v1.x != v2.x || v1.y != v2.y;
}

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE Vec3<T>::Vec3()
	: x(T(0))
	, y(T(0))
	, z(T(0))
{

}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>::Vec3(const Vec3<T>& v)
	: x(v.x)
	, y(v.y)
	, z(v.z)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>::Vec3(const Vec4<T>& v)
	: x(v.x)
	, y(v.y)
	, z(v.z)
{

}

template <typename T>
template <typename T2>
HINATA_FORCE_INLINE Vec3<T>::Vec3(const Vec3<T2>& v)
	: x(T(v.x))
	, y(T(v.y))
	, z(T(v.z))
{

}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>::Vec3(T v)
	: x(v)
	, y(v)
	, z(v)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>::Vec3(T x, T y, T z)
	: x(x)
	, y(y)
	, z(z)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>::Vec3(T x, const Vec2<T>& v)
	: x(x)
	, y(v.x)
	, z(v.y)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>::Vec3(const Vec2<T>& v, T z)
	: x(v.x)
	, y(v.y)
	, z(z)
{

}

template <typename T>
HINATA_FORCE_INLINE T& Vec3<T>::operator[](int i)
{
	return (&x)[i];
}

template <typename T>
HINATA_FORCE_INLINE const T& Vec3<T>::operator[](int i) const
{
	return (&x)[i];
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>& Vec3<T>::operator=(const Vec3<T>& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>& Vec3<T>::operator*=(const Vec3<T>& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>& Vec3<T>::operator/=(const Vec3<T>& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>& Vec3<T>::operator++()
{
	++x;
	++y;
	++z;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> Vec3<T>::operator++(int)
{
	return Vec3<T>(x + T(1), y + T(1), z + T(1));
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>& Vec3<T>::operator--()
{
	--x;
	--y;
	--z;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> Vec3<T>::operator--(int)
{
	return Vec3<T>(x - T(1), y - T(1), z - T(1));
}

// Binary + operator
template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator+(const Vec3<T>& v, T s)
{
	return Vec3<T>(v.x + s, v.y + s, v.z + s);
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator+(T s, const Vec3<T>& v)
{
	return Vec3<T>(s + v.x, s + v.y, s + v.z);
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator+(const Vec3<T>& v1, const Vec3<T>& v2)
{
	return Vec3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

// Binary - operator
template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator-(const Vec3<T>& v, T s)
{
	return Vec3<T>(v.x - s, v.y - s, v.z - s);
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator-(T s, const Vec3<T>& v)
{
	return Vec3<T>(s - v.x, s - v.y, s - v.z);
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator-(const Vec3<T>& v1, const Vec3<T>& v2)
{
	return Vec3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

// Binary * operator
template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator*(const Vec3<T>& v, T s)
{
	return Vec3<T>(v.x * s, v.y * s, v.z * s);
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator*(T s, const Vec3<T>& v)
{
	return Vec3<T>(s * v.x, s * v.y, s * v.z);
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator*(const Vec3<T>& v1, const Vec3<T>& v2)
{
	return Vec3<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

// Binary / operator
template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator/(const Vec3<T>& v, T s)
{
	return Vec3<T>(v.x / s, v.y / s, v.z / s);
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator/(T s, const Vec3<T>& v)
{
	return Vec3<T>(s / v.x, s / v.y, s / v.z);
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator/(const Vec3<T>& v1, const Vec3<T>& v2)
{
	return Vec3<T>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

// Unary - operator
template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator-(const Vec3<T>& v)
{
	return Vec3<T>(-v.x, -v.y, -v.z);
}

// Boolean operators
template <typename T>
HINATA_FORCE_INLINE bool operator==(const Vec3<T>& v1, const Vec3<T>& v2)
{
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

template <typename T>
HINATA_FORCE_INLINE bool operator!=(const Vec3<T>& v1, const Vec3<T>& v2)
{
	return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
}

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE Vec4<T>::Vec4()
	: x(T(0))
	, y(T(0))
	, z(T(0))
	, w(T(0))
{

}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>::Vec4(const Vec4<T>& v)
	: x(v.x)
	, y(v.y)
	, z(v.z)
	, w(v.w)
{

}

template <typename T>
template <typename T2>
HINATA_FORCE_INLINE Vec4<T>::Vec4(const Vec4<T2>& v)
	: x(T(v.x))
	, y(T(v.y))
	, z(T(v.z))
	, w(T(v.w))
{

}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>::Vec4(T v)
	: x(v)
	, y(v)
	, z(v)
	, w(v)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>::Vec4(T x, T y, T z, T w)
	: x(x)
	, y(y)
	, z(z)
	, w(w)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>::Vec4(T x, const Vec3<T>& v)
	: x(x)
	, y(v.x)
	, z(v.y)
	, w(v.z)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>::Vec4(const Vec3<T>& v, T w)
	: x(v.x)
	, y(v.y)
	, z(v.z)
	, w(w)
{
	
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>::Vec4(T x, T y, const Vec2<T>& v)
	: x(x)
	, y(y)
	, z(v.x)
	, w(v.y)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>::Vec4(T x, const Vec2<T>& v, T w)
	: x(x)
	, y(v.x)
	, z(v.y)
	, w(w)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>::Vec4(const Vec2<T>& v, T z, T w)
	: x(v.x)
	, y(v.y)
	, z(z)
	, w(w)
{

}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>::Vec4(const Vec2<T>& v1, const Vec2<T>& v2)
	: x(v1.x)
	, y(v1.y)
	, z(v2.x)
	, w(v2.y)
{

}

template <typename T>
HINATA_FORCE_INLINE T& Vec4<T>::operator[](int i)
{
	return (&x)[i];
}

template <typename T>
HINATA_FORCE_INLINE const T& Vec4<T>::operator[](int i) const
{
	return (&x)[i];
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>& Vec4<T>::operator=(const Vec4<T>& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>& Vec4<T>::operator*=(const Vec4<T>& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>& Vec4<T>::operator/=(const Vec4<T>& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>& Vec4<T>::operator++()
{
	++x;
	++y;
	++z;
	++w;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> Vec4<T>::operator++(int)
{
	return Vec4<T>(x + T(1), y + T(1), z + T(1), w + T(1));
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>& Vec4<T>::operator--()
{
	--x;
	--y;
	--z;
	--w;
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> Vec4<T>::operator--(int)
{
	return Vec4<T>(x - T(1), y - T(1), z - T(1), w - T(1));
}

// Binary + operator
template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator+(const Vec4<T>& v, T s)
{
	return Vec4<T>(v.x + s, v.y + s, v.z + s, v.w + s);
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator+(T s, const Vec4<T>& v)
{
	return Vec4<T>(s + v.x, s + v.y, s + v.z, v.w + s);
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator+(const Vec4<T>& v1, const Vec4<T>& v2)
{
	return Vec4<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

// Binary - operator
template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator-(const Vec4<T>& v, T s)
{
	return Vec4<T>(v.x - s, v.y - s, v.z - s, v.w - s);
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator-(T s, const Vec4<T>& v)
{
	return Vec4<T>(s - v.x, s - v.y, s - v.z, s - v.w);
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator-(const Vec4<T>& v1, const Vec4<T>& v2)
{
	return Vec4<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

// Binary * operator
template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator*(const Vec4<T>& v, T s)
{
	return Vec4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator*(T s, const Vec4<T>& v)
{
	return Vec4<T>(s * v.x, s * v.y, s * v.z, s * v.w);
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator*(const Vec4<T>& v1, const Vec4<T>& v2)
{
	return Vec4<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

// Binary / operator
template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator/(const Vec4<T>& v, T s)
{
	return Vec4<T>(v.x / s, v.y / s, v.z / s, v.w / s);
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator/(T s, const Vec4<T>& v)
{
	return Vec4<T>(s / v.x, s / v.y, s / v.z, s / v.w);
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator/(const Vec4<T>& v1, const Vec4<T>& v2)
{
	return Vec4<T>(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

// Unary - operator
template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator-(const Vec4<T>& v)
{
	return Vec4<T>(-v.x, -v.y, -v.z, -v.w);
}

// Boolean operators
template <typename T>
HINATA_FORCE_INLINE bool operator==(const Vec4<T>& v1, const Vec4<T>& v2)
{
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
}

template <typename T>
HINATA_FORCE_INLINE bool operator!=(const Vec4<T>& v1, const Vec4<T>& v2)
{
	return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z || v1.w != v2.w;
}

HINATA_NAMESPACE_END