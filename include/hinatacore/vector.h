#ifndef __HINATA_CORE_VECTOR_H__
#define __HINATA_CORE_VECTOR_H__

#include "common.h"

HINATA_NAMESPACE_BEGIN

/*!
	2D vector.
	\tparam T Internal value type.
*/
template <typename T>
struct Vec2
{

	union
	{
		struct { T x, y; };
		struct { T r, g; };
		struct { T s, t; };
	};

	// Constructors
	HINATA_FORCE_INLINE Vec2();
	HINATA_FORCE_INLINE Vec2(const Vec2<T>& v);
	HINATA_FORCE_INLINE explicit Vec2(T v);
	HINATA_FORCE_INLINE Vec2(T x, T y);

	// Accessors
	HINATA_FORCE_INLINE T& operator[](int i);
	HINATA_FORCE_INLINE const T& operator[](int i) const;

	// Operators
	HINATA_FORCE_INLINE Vec2<T>& operator=(const Vec2<T>& v);
	HINATA_FORCE_INLINE Vec2<T>& operator+=(const Vec2<T>& v);
	HINATA_FORCE_INLINE Vec2<T>& operator-=(const Vec2<T>& v);
	HINATA_FORCE_INLINE Vec2<T>& operator*=(const Vec2<T>& v);
	HINATA_FORCE_INLINE Vec2<T>& operator/=(const Vec2<T>& v);
	HINATA_FORCE_INLINE Vec2<T>& operator++();
	HINATA_FORCE_INLINE Vec2<T> operator++(int);
	HINATA_FORCE_INLINE Vec2<T>& operator--();
	HINATA_FORCE_INLINE Vec2<T> operator--(int);

};

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec2<int> Vec2i;

// --------------------------------------------------------------------------------

/*!
	3D vector.
	\tparam T Internal value type.
*/
template <typename T>
struct Vec3
{
	
	union
	{
		struct { T x, y, z; };
		struct { T r, g, b; };
		struct { T s, t, p; };
	};

	// Constructors
	HINATA_FORCE_INLINE Vec3();
	HINATA_FORCE_INLINE Vec3(const Vec3<T>& v);
	HINATA_FORCE_INLINE explicit Vec3(T v);
	HINATA_FORCE_INLINE Vec3(T x, T y, T z);
	HINATA_FORCE_INLINE Vec3(T x, const Vec2<T>& v);
	HINATA_FORCE_INLINE Vec3(const Vec2<T>& v, T z);

	// Accessors
	HINATA_FORCE_INLINE T& operator[](int i);
	HINATA_FORCE_INLINE const T& operator[](int i) const;

	// Operators
	HINATA_FORCE_INLINE Vec3<T>& operator=(const Vec3<T>& v);
	HINATA_FORCE_INLINE Vec3<T>& operator+=(const Vec3<T>& v);
	HINATA_FORCE_INLINE Vec3<T>& operator-=(const Vec3<T>& v);
	HINATA_FORCE_INLINE Vec3<T>& operator*=(const Vec3<T>& v);
	HINATA_FORCE_INLINE Vec3<T>& operator/=(const Vec3<T>& v);
	HINATA_FORCE_INLINE Vec3<T>& operator++();
	HINATA_FORCE_INLINE Vec3<T> operator++(int);
	HINATA_FORCE_INLINE Vec3<T>& operator--();
	HINATA_FORCE_INLINE Vec3<T> operator--(int);

};

typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<int> Vec3i;

// --------------------------------------------------------------------------------

/*!
	4D vector.
	\tparam T Internal value type
*/
template <typename T>
struct Vec4
{

	union
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		struct { T s, t, p, q; };
	};

	// Constructors
	HINATA_FORCE_INLINE Vec4();
	HINATA_FORCE_INLINE Vec4(const Vec4<T>& v);
	HINATA_FORCE_INLINE explicit Vec4(T v);
	HINATA_FORCE_INLINE Vec4(T x, T y, T z, T w);
	HINATA_FORCE_INLINE Vec4(T x, const Vec3<T>& v);
	HINATA_FORCE_INLINE Vec4(const Vec3<T>& v, T w);
	HINATA_FORCE_INLINE Vec4(T x, T y, const Vec2<T>& v);
	HINATA_FORCE_INLINE Vec4(T x, const Vec2<T>& v, T w);
	HINATA_FORCE_INLINE Vec4(const Vec2<T>& v, T z, T w);
	HINATA_FORCE_INLINE Vec4(const Vec2<T>& v1, const Vec2<T>& v2);

	// Accessors
	HINATA_FORCE_INLINE T& operator[](int i);
	HINATA_FORCE_INLINE const T& operator[](int i) const;

	// Operators
	HINATA_FORCE_INLINE Vec4<T>& operator=(const Vec4<T>& v);
	HINATA_FORCE_INLINE Vec4<T>& operator+=(const Vec4<T>& v);
	HINATA_FORCE_INLINE Vec4<T>& operator-=(const Vec4<T>& v);
	HINATA_FORCE_INLINE Vec4<T>& operator*=(const Vec4<T>& v);
	HINATA_FORCE_INLINE Vec4<T>& operator/=(const Vec4<T>& v);
	HINATA_FORCE_INLINE Vec4<T>& operator++();
	HINATA_FORCE_INLINE Vec4<T> operator++(int);
	HINATA_FORCE_INLINE Vec4<T>& operator--();
	HINATA_FORCE_INLINE Vec4<T> operator--(int);

};

typedef Vec4<float> Vec4f;
typedef Vec4<double> Vec4d;
typedef Vec4<int> Vec4i;

HINATA_NAMESPACE_END

#include "vector.inl"

#endif // __HINATA_CORE_VECTOR_H__
