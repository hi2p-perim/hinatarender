#ifndef __HINATA_CORE_MATRIX_H__
#define __HINATA_CORE_MATRIX_H__

#include "common.h"

HINATA_NAMESPACE_BEGIN

template <typename T> struct Vec3;
template <typename T> struct Vec4;
template <typename T> struct Mat4;

/*!
	3x3 matrix.
	Column major 3x3 matrix. 
	A matrix
		v00 v01 v02
		v10 v11 v12
		v20 v21 v22
	is stored sequentially as v00, v10, ..., v22.
	\tparam T Internal value type.
*/
template <typename T>
struct Mat3
{

	Vec3<T> data[3];

	// Constructors
	HINATA_FORCE_INLINE Mat3();
	HINATA_FORCE_INLINE Mat3(const Mat3<T>& m);
	HINATA_FORCE_INLINE Mat3(const Mat4<T>& m);
	template <typename T2> HINATA_FORCE_INLINE Mat3(const Mat3<T2>& m);
	HINATA_FORCE_INLINE explicit Mat3(T v);
	HINATA_FORCE_INLINE Mat3(const Vec3<T>& v0, const Vec3<T>& v1, const Vec3<T>& v2);
	HINATA_FORCE_INLINE Mat3(
		T v00, T v10, T v20,
		T v01, T v11, T v21,
		T v02, T v12, T v22);

	// Constants
	static HINATA_FORCE_INLINE Mat3<T> Zero();
	static HINATA_FORCE_INLINE Mat3<T> Diag(T v);
	static HINATA_FORCE_INLINE Mat3<T> Identity();

	// Accessors
	HINATA_FORCE_INLINE Vec3<T>& operator[](int i);
	HINATA_FORCE_INLINE const Vec3<T>& operator[](int i) const;

	// Operators
	HINATA_FORCE_INLINE Mat3<T>& operator=(const Mat3<T>& m);
	HINATA_FORCE_INLINE Mat3<T>& operator+=(const Mat3<T>& m);
	HINATA_FORCE_INLINE Mat3<T>& operator-=(const Mat3<T>& m);
	HINATA_FORCE_INLINE Mat3<T>& operator*=(const Mat3<T>& m);
	HINATA_FORCE_INLINE Mat3<T>& operator/=(const Mat3<T>& m);
	HINATA_FORCE_INLINE Mat3<T>& operator++();
	HINATA_FORCE_INLINE Mat3<T> operator++(int);
	HINATA_FORCE_INLINE Mat3<T>& operator--();
	HINATA_FORCE_INLINE Mat3<T> operator--(int);

};

typedef Mat3<int> Mat3i;
typedef Mat3<float> Mat3f;
typedef Mat3<double> Mat3d;

// ------------------------------------------------------------------------------------------

/*!
	4x4 matrix.
	Column major 4x4 matrix. 
	A matrix
		v00 v01 v02 v03
		v10 v11 v12 v13
		v20 v21 v22 v23
		v30 v31 v32 v33
	is stored sequentially as v00, v10, ..., v33.
	\tparam T Internal value type.
*/
template <typename T>
struct Mat4
{

	Vec4<T> data[4];

	// Constructors
	HINATA_FORCE_INLINE Mat4();
	HINATA_FORCE_INLINE Mat4(const Mat4<T>& m);
	template <typename T2> HINATA_FORCE_INLINE Mat4(const Mat4<T2>& m);
	HINATA_FORCE_INLINE explicit Mat4(T v);
	HINATA_FORCE_INLINE Mat4(const Vec4<T>& v0, const Vec4<T>& v1, const Vec4<T>& v2, const Vec4<T>& v3);
	HINATA_FORCE_INLINE Mat4(
		T v00, T v10, T v20, T v30,
		T v01, T v11, T v21, T v31,
		T v02, T v12, T v22, T v32,
		T v03, T v13, T v23, T v33);

	// Constants
	static HINATA_FORCE_INLINE Mat4<T> Zero();
	static HINATA_FORCE_INLINE Mat4<T> Diag(T v);
	static HINATA_FORCE_INLINE Mat4<T> Identity();

	// Accessors
	HINATA_FORCE_INLINE Vec4<T>& operator[](int i);
	HINATA_FORCE_INLINE const Vec4<T>& operator[](int i) const;

	// Operators
	HINATA_FORCE_INLINE Mat4<T>& operator=(const Mat4<T>& m);
	HINATA_FORCE_INLINE Mat4<T>& operator+=(const Mat4<T>& m);
	HINATA_FORCE_INLINE Mat4<T>& operator-=(const Mat4<T>& m);
	HINATA_FORCE_INLINE Mat4<T>& operator*=(const Mat4<T>& m);
	HINATA_FORCE_INLINE Mat4<T>& operator/=(const Mat4<T>& m);
	HINATA_FORCE_INLINE Mat4<T>& operator++();
	HINATA_FORCE_INLINE Mat4<T> operator++(int);
	HINATA_FORCE_INLINE Mat4<T>& operator--();
	HINATA_FORCE_INLINE Mat4<T> operator--(int);

};

typedef Mat4<int> Mat4i;
typedef Mat4<float> Mat4f;
typedef Mat4<double> Mat4d;

HINATA_NAMESPACE_END

#include "matrix.inl"

#endif // __HINATA_CORE_MATRIX_H__