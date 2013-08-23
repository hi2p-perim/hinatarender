#include "common.h"
#include "mathconsts.h"

HINATA_NAMESPACE_BEGIN

namespace Math
{

template <typename T>
HINATA_FORCE_INLINE T Radians(T v)
{
	return v * T(Pi) / T(180);
}

template <typename T>
HINATA_FORCE_INLINE T Degrees(T v)
{
	return v * T(180) / T(Pi);
}

// ------------------------------------------------------------------------------------------

template <typename T> HINATA_FORCE_INLINE
Mat4<T> ConvertToMat4( const T* p )
{
	Mat4<T> r;
	memcpy(&(r[0].x), p, sizeof(Mat4<T>));
	return r;
}

template <typename T> HINATA_FORCE_INLINE
Vec3<T> ConvertToVec3( const T* p )
{
	Vec3<T> r;
	memcpy(&(r.x), p, sizeof(Vec3<T>));
	return r;
}

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Length(T v)
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

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Length2(T v)
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

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Normalize(T v)
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

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Dot(T v1, T v2)
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

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE Vec3<T> Cross(const Vec3<T>& v1, const Vec3<T>& v2)
{
	return Vec3<T>(
		v1.y * v2.z - v2.y * v1.z,
		v1.z * v2.x - v2.z * v1.x,
		v1.x * v2.y - v2.x * v1.y);
}

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Fract(T v)
{
	return v - std::floor(v);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> Fract(const Vec2<T>& v)
{
	return Vec2<T>(Fract(v.x), Fract(v.y));
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> Fract(const Vec3<T>& v)
{
	return Vec3<T>(Fract(v.x), Fract(v.y), Fract(v.z));
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> Fract(const Vec4<T>& v)
{
	return Vec4<T>(Fract(v.x), Fract(v.y), Fract(v.z), Fract(v.w));
}

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Clamp(T v, T min, T max)
{
	return std::min(std::max(v, min), max);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> Clamp(const Vec2<T>& v, T min, T max)
{
	return Vec2<T>(
		Clamp(v.x, min, max),
		Clamp(v.y, min, max));
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> Clamp(const Vec3<T>& v, T min, T max)
{
	return Vec3<T>(
		Clamp(v.x, min, max),
		Clamp(v.y, min, max),
		Clamp(v.z, min, max));
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> Clamp(const Vec4<T>& v, T min, T max)
{
	return Vec4<T>(
		Clamp(v.x, min, max),
		Clamp(v.y, min, max),
		Clamp(v.z, min, max),
		Clamp(v.w, min, max));
}

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Min(T v1, T v2)
{
	return std::min(v1, v2);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> Min(const Vec2<T>& v1, const Vec2<T>& v2)
{
	return Vec2<T>(Min(v1.x, v2.x), Min(v1.y, v2.y));
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> Min(const Vec3<T>& v1, const Vec3<T>& v2)
{
	return Vec3<T>(Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z));
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> Min(const Vec4<T>& v1, const Vec4<T>& v2)
{
	return Vec4<T>(Min(v1.x, v2.x), Min(v1.y, v2.y), Min(v1.z, v2.z), Min(v1.w, v2.w));
}

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Max(T v1, T v2)
{
	return std::max(v1, v2);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> Max(const Vec2<T>& v1, const Vec2<T>& v2)
{
	return Vec2<T>(Max(v1.x, v2.x), Max(v1.y, v2.y));
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> Max(const Vec3<T>& v1, const Vec3<T>& v2)
{
	return Vec3<T>(Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z));
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> Max(const Vec4<T>& v1, const Vec4<T>& v2)
{
	return Vec4<T>(Max(v1.x, v2.x), Max(v1.y, v2.y), Max(v1.z, v2.z), Max(v1.w, v2.w));
}

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE T Abs(T v)
{
	return std::abs(v);
}

template <typename T>
HINATA_FORCE_INLINE Vec2<T> Abs(const Vec2<T>& v)
{
	return Vec2<T>(Abs(v.x), Abs(v.y));
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> Abs(const Vec3<T>& v)
{
	return Vec3<T>(Abs(v.x), Abs(v.y), Abs(v.z));
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> Abs(const Vec4<T>& v)
{
	return Vec4<T>(Abs(v.x), Abs(v.y), Abs(v.z), Abs(v.w));
}

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE Mat3<T> Transpose(const Mat3<T>& m)
{
	return Mat3<T>(
		m[0][0], m[1][0], m[2][0],
		m[0][1], m[1][1], m[2][1],
		m[0][2], m[1][2], m[2][2]);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Transpose(const Mat4<T>& m)
{
	return Mat4<T>(
		m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3]);
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> Inverse(const Mat3<T>& m)
{
	T det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
		- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
		+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);

	Mat3<T> r;
	r[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]);
	r[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]);
	r[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]);
	r[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]);
	r[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]);
	r[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]);
	r[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]);
	r[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]);
	r[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]);

	return r / det;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Inverse(const Mat4<T>& m)
{
	T c00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	T c02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	T c03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
	T c04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	T c06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	T c07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
	T c08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	T c10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	T c11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	T c12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	T c14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	T c15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
	T c16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	T c18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	T c19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
	T c20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	T c22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	T c23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	Vec4<T> f0(c00, c00, c02, c03);
	Vec4<T> f1(c04, c04, c06, c07);
	Vec4<T> f2(c08, c08, c10, c11);
	Vec4<T> f3(c12, c12, c14, c15);
	Vec4<T> f4(c16, c16, c18, c19);
	Vec4<T> f5(c20, c20, c22, c23);

	Vec4<T> v0(m[1][0], m[0][0], m[0][0], m[0][0]);
	Vec4<T> v1(m[1][1], m[0][1], m[0][1], m[0][1]);
	Vec4<T> v2(m[1][2], m[0][2], m[0][2], m[0][2]);
	Vec4<T> v3(m[1][3], m[0][3], m[0][3], m[0][3]);

	const Vec4<T> sA(+1, -1, +1, -1);
	const Vec4<T> sB(-1, +1, -1, +1);

	auto inv_v0 = sA * (v1 * f0 - v2 * f1 + v3 * f2);
	auto inv_v1 = sB * (v0 * f0 - v2 * f3 + v3 * f4);
	auto inv_v2 = sA * (v0 * f1 - v1 * f3 + v3 * f5);
	auto inv_v3 = sB * (v0 * f2 - v1 * f4 + v2 * f5);

	Mat4<T> inv(inv_v0, inv_v1, inv_v2, inv_v3);
	T det = Math::Dot(m[0], Vec4<T>(inv[0][0], inv[1][0], inv[2][0], inv[3][0]));

	return inv / det;
}

// ------------------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Translate(const Mat4<T>& m, const Vec3<T>& v)
{
	Mat4<T> r(m);
	r[3] = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w;
	return r;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Translate(const Vec3<T>& v)
{
	return Translate(Mat4d::Identity(), v);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Rotate(const Mat4<T>& m, T angle, const Vec3<T>& axis)
{
	T c = std::cos(Radians(angle));
	T s = std::sin(Radians(angle));

	auto a = Normalize(axis);
	auto t = (T(1) - c) * a;

	Mat4<T> rot;
	rot[0][0] = c + t[0] * a[0];
	rot[0][1] = 0 + t[0] * a[1] + s * a[2];
	rot[0][2] = 0 + t[0] * a[2] - s * a[1];
	rot[1][0] = 0 + t[1] * a[0] - s * a[2];
	rot[1][1] = c + t[1] * a[1];
	rot[1][2] = 0 + t[1] * a[2] + s * a[0];
	rot[2][0] = 0 + t[2] * a[0] + s * a[1];
	rot[2][1] = 0 + t[2] * a[1] - s * a[0];
	rot[2][2] = c + t[2] * a[2];

	Mat4<T> r;
	r[0] = m[0] * rot[0][0] + m[1] * rot[0][1] + m[2] * rot[0][2];
	r[1] = m[0] * rot[1][0] + m[1] * rot[1][1] + m[2] * rot[1][2];
	r[2] = m[0] * rot[2][0] + m[1] * rot[2][1] + m[2] * rot[2][2];
	r[3] = m[3];

	return r;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Rotate(T angle, const Vec3<T>& axis)
{
	return Rotate(Mat4<T>::Identity(), angle, axis);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Scale(const Mat4<T>& m, const Vec3<T>& v)
{
	return Mat4<T>(m[0] * v[0], m[1] * v[1], m[2] * v[2], m[3]);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Scale(const Vec3<T>& v)
{
	return Scale(Mat4<T>::Identity(), v);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> LookAt(const Vec3<T>& eye, const Vec3<T>& center, const Vec3<T>& up)
{
	auto f = Normalize(center - eye);
	auto u = Normalize(up);
	auto s = Normalize(Cross(f, u));
	u = Cross(s, f);

	auto r = Mat4<T>::Identity();
	r[0][0] = s.x;
	r[1][0] = s.y;
	r[2][0] = s.z;
	r[0][1] = u.x;
	r[1][1] = u.y;
	r[2][1] = u.z;
	r[0][2] = -f.x;
	r[1][2] = -f.y;
	r[2][2] = -f.z;
	r[3][0] = -Dot(s, eye);
	r[3][1] = -Dot(u, eye);
	r[3][2] = Dot(f, eye);

	return r;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Perspective(T fovy, T aspect, T zNear, T zFar)
{
	T radian = Radians(fovy);
	T t = std::tan(radian / T(2));

	Mat4<T> r;
	r[0][0] = T(1) / (aspect * t);
	r[1][1] = T(1) / t;
	r[2][2] = -(zFar + zNear) / (zFar - zNear);
	r[2][3] = -T(1);
	r[3][2] = -(T(2) * zFar * zNear) / (zFar - zNear);

	return r;
}

} // namespace Math

HINATA_NAMESPACE_END