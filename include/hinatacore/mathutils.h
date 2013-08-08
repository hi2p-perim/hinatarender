#ifndef __HINATA_CORE_MATH_UTILS_H__
#define __HINATA_CORE_MATH_UTILS_H__

#include "common.h"
#include <cmath>

HINATA_NAMESPACE_BEGIN

template <typename T> HINATA_FORCE_INLINE T Length(const T& v);
template <typename T> HINATA_FORCE_INLINE T Length(const Vec2<T>& v);
template <typename T> HINATA_FORCE_INLINE T Length(const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE T Length(const Vec4<T>& v);

template <typename T> HINATA_FORCE_INLINE T Length2(const T& v);
template <typename T> HINATA_FORCE_INLINE T Length2(const Vec2<T>& v);
template <typename T> HINATA_FORCE_INLINE T Length2(const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE T Length2(const Vec4<T>& v);

template <typename T> HINATA_FORCE_INLINE T Normalize(const T& v);
template <typename T> HINATA_FORCE_INLINE Vec2<T> Normalize(const Vec2<T>& v);
template <typename T> HINATA_FORCE_INLINE Vec3<T> Normalize(const Vec3<T>& v);
template <typename T> HINATA_FORCE_INLINE Vec4<T> Normalize(const Vec4<T>& v);

template <typename T> HINATA_FORCE_INLINE T Dot(const T& v1, const T& v2);
template <typename T> HINATA_FORCE_INLINE T Dot(const Vec2<T>& v1, const Vec2<T>& v2);
template <typename T> HINATA_FORCE_INLINE T Dot(const Vec3<T>& v1, const Vec3<T>& v2);
template <typename T> HINATA_FORCE_INLINE T Dot(const Vec4<T>& v1, const Vec4<T>& v2);

template <typename T> HINATA_FORCE_INLINE Vec3<T> Cross(const Vec3<T>& v1, const Vec3<T>& v2);

HINATA_NAMESPACE_END

#include "mathutils.inl"

#endif // __HINATA_CORE_MATH_UTILS_H__