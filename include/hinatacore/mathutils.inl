#include "common.h"

HINATA_NAMESPACE_BEGIN

template <typename T>
HINATA_FORCE_INLINE T Length(const T& v)
{
	return std::abs(v);
}

template <typename T>
HINATA_FORCE_INLINE T Length(const Vec2<T>& v)
{
	return std::sqrt(Length2(v));
}

template <typename T>
HINATA_FORCE_INLINE T Length(const Vec3<T>& v)
{
	return std::sqrt(Length2(v));
}

template <typename T>
HINATA_FORCE_INLINE T Length(const Vec4<T>& v)
{
	return std::sqrt(Length2(v));
}

// --------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Length2(const T& v)
{
	return v * v;
}

template <typename T>
HINATA_FORCE_INLINE T Length2(const Vec2<T>& v)
{
	return v.x * v.x + v.y * v.y;
}

template <typename T>
HINATA_FORCE_INLINE T Length2(const Vec3<T>& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

template <typename T>
HINATA_FORCE_INLINE T Length2(const Vec4<T>& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

// --------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Normalize(const T& v)
{
	return v < T(0) ? T(-1) : T(1);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> Normalize(const Vec2<T>& v)
{
	return v / Length(v);
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> Normalize(const Vec3<T>& v)
{
	return v / Length(v);
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> Normalize(const Vec4<T>& v)
{
	return v / Length(v);
}

// --------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Dot(const T& v1, const T& v2)
{
	return v1 * v2;
}

template <typename T>
HINATA_FORCE_INLINE T Dot(const Vec2<T>& v1, const Vec2<T>& v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
HINATA_FORCE_INLINE T Dot(const Vec3<T>& v1, const Vec3<T>& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
HINATA_FORCE_INLINE T Dot(const Vec4<T>& v1, const Vec4<T>& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

// --------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE Vec3<T> Cross(const Vec3<T>& v1, const Vec3<T>& v2)
{
	return Vec3<T>(
		v1.y * v2.z - v2.y * v1.z,
		v1.z * v2.x - v2.z * v1.x,
		v1.x * v2.y - v2.x * v1.y);
}

HINATA_NAMESPACE_END