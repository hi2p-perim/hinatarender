#ifndef __HINATA_CORE_MATH_FUNCS_H__
#define __HINATA_CORE_MATH_FUNCS_H__

#include "common.h"
#include "vector.h"
#include "matrix.h"
#include <cmath>

HINATA_NAMESPACE_BEGIN

namespace Math
{

// Angle conversion
template <typename T> HINATA_FORCE_INLINE T Radians(T v);
template <typename T> HINATA_FORCE_INLINE T Degrees(T v);

// ------------------------------------------------------------------------------------------

template <typename T> HINATA_FORCE_INLINE Mat4<T> ConvertToMat4(const T* p);
template <typename T> HINATA_FORCE_INLINE Vec3<T> ConvertToVec3(const T* p);

// ------------------------------------------------------------------------------------------

// Vector operations

template <typename T> HINATA_FORCE_INLINE T Length(T v);
template <typename T> HINATA_FORCE_INLINE T Length(const Vec2<T>& v);
template <typename T> HINATA_FORCE_INLINE T Length(const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE T Length(const Vec4<T>& v);

template <typename T> HINATA_FORCE_INLINE T Length2(T v);
template <typename T> HINATA_FORCE_INLINE T Length2(const Vec2<T>& v);
template <typename T> HINATA_FORCE_INLINE T Length2(const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE T Length2(const Vec4<T>& v);

template <typename T> HINATA_FORCE_INLINE T Normalize(T v);
template <typename T> HINATA_FORCE_INLINE Vec2<T> Normalize(const Vec2<T>& v);
template <typename T> HINATA_FORCE_INLINE Vec3<T> Normalize(const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE Vec4<T> Normalize(const Vec4<T>& v);

template <typename T> HINATA_FORCE_INLINE T Dot(T v1, T v2);
template <typename T> HINATA_FORCE_INLINE T Dot(const Vec2<T>& v1, const Vec2<T>& v2);
template <typename T> HINATA_FORCE_INLINE T Dot(const Vec3<T>& v1, const Vec3<T>& v2);
template <typename T> HINATA_FORCE_INLINE T Dot(const Vec4<T>& v1, const Vec4<T>& v2);

template <typename T> HINATA_FORCE_INLINE Vec3<T> Cross(const Vec3<T>& v1, const Vec3<T>& v2);

// ------------------------------------------------------------------------------------------

// Per-component operations

template <typename T> HINATA_FORCE_INLINE T Fract(T v);
template <typename T> HINATA_FORCE_INLINE Vec2<T> Fract(const Vec2<T>& v);
template <typename T> HINATA_FORCE_INLINE Vec3<T> Fract(const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE Vec4<T> Fract(const Vec4<T>& v);

template <typename T> HINATA_FORCE_INLINE T Clamp(T v, T min, T max);
template <typename T> HINATA_FORCE_INLINE Vec2<T> Clamp(const Vec2<T>& v, T min, T max);
template <typename T> HINATA_FORCE_INLINE Vec3<T> Clamp(const Vec3<T>& v, T min, T max);
template <typename T> HINATA_FORCE_INLINE Vec4<T> Clamp(const Vec4<T>& v, T min, T max);

template <typename T> HINATA_FORCE_INLINE T Min(T v1, T v2);
template <typename T> HINATA_FORCE_INLINE Vec2<T> Min(const Vec2<T>& v1, const Vec2<T>& v2);
template <typename T> HINATA_FORCE_INLINE Vec3<T> Min(const Vec2<T>& v1, const Vec2<T>& v2);
template <typename T> HINATA_FORCE_INLINE Vec4<T> Min(const Vec2<T>& v1, const Vec2<T>& v2);

template <typename T> HINATA_FORCE_INLINE T Max(T v1, T v2);
template <typename T> HINATA_FORCE_INLINE Vec2<T> Max(const Vec2<T>& v1, const Vec2<T>& v2);
template <typename T> HINATA_FORCE_INLINE Vec3<T> Max(const Vec3<T>& v1, const Vec3<T>& v2);
template <typename T> HINATA_FORCE_INLINE Vec4<T> Max(const Vec4<T>& v1, const Vec4<T>& v2);

template <typename T> HINATA_FORCE_INLINE T Abs(T v);
template <typename T> HINATA_FORCE_INLINE Vec2<T> Abs(const Vec2<T>& v);
template <typename T> HINATA_FORCE_INLINE Vec3<T> Abs(const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE Vec4<T> Abs(const Vec4<T>& v);

// ------------------------------------------------------------------------------------------

// Matrix operations

template <typename T> HINATA_FORCE_INLINE Mat3<T> Transpose(const Mat3<T>& m);
template <typename T> HINATA_FORCE_INLINE Mat4<T> Transpose(const Mat4<T>& m);

template <typename T> HINATA_FORCE_INLINE Mat3<T> Inverse(const Mat3<T>& m);
template <typename T> HINATA_FORCE_INLINE Mat4<T> Inverse(const Mat4<T>& m);

// ------------------------------------------------------------------------------------------

// Transformations

template <typename T> HINATA_FORCE_INLINE Mat4<T> Translate(const Mat4<T>& m, const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE Mat4<T> Translate(const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE Mat4<T> Rotate(const Mat4<T>& m, T angle, const Vec3<T>& axis);
template <typename T> HINATA_FORCE_INLINE Mat4<T> Rotate(T angle, const Vec3<T>& axis);
template <typename T> HINATA_FORCE_INLINE Mat4<T> Scale(const Mat4<T>& m, const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE Mat4<T> Scale(const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE Mat4<T> LookAt(const Vec3<T>& eye, const Vec3<T>& center, const Vec3<T>& up);
template <typename T> HINATA_FORCE_INLINE Mat4<T> Perspective(T fovy, T aspect, T zNear, T zFar);

} // namespace Math

HINATA_NAMESPACE_END

#include "mathfuncs.inl"

#endif // __HINATA_CORE_MATH_FUNCS_H__
