#include "common.h"

HINATA_NAMESPACE_BEGIN

template <typename T>
HINATA_FORCE_INLINE Mat3<T>::Mat3()
{

}

template <typename T>
HINATA_FORCE_INLINE Mat3<T>::Mat3(const Mat3<T>& m)
{
	data[0] = m.data[0];
	data[1] = m.data[1];
	data[2] = m.data[2];
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T>::Mat3(T v)
{
	data[0] = Vec3<T>(v);
	data[1] = Vec3<T>(v);
	data[2] = Vec3<T>(v);
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T>::Mat3(const Vec3<T>& v0, const Vec3<T>& v1, const Vec3<T>& v2)
{
	data[0] = v0;
	data[1] = v1;
	data[2] = v2;
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T>::Mat3(
	T v00, T v10, T v20,
	T v01, T v11, T v21,
	T v02, T v12, T v22)
{
	data[0] = Vec3<T>(v00, v10, v20);
	data[1] = Vec3<T>(v01, v11, v21);
	data[2] = Vec3<T>(v02, v12, v22);
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> Mat3<T>::Zero()
{
	return Mat3<T>();
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> Mat3<T>::Diag(T v)
{
	return Mat3<T>(
		Vec3<T>(v, T(0), T(0)),
		Vec3<T>(T(0), v, T(0)),
		Vec3<T>(T(0), T(0), v));
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> Mat3<T>::Identity()
{
	return Diag(T(1));
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T>& Mat3<T>::operator[](int i)
{
	return data[i];
}

template <typename T>
HINATA_FORCE_INLINE const Vec3<T>& Mat3<T>::operator[](int i) const
{
	return data[i];
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T>& Mat3<T>::operator=(const Mat3<T>& m)
{
	data[0] = m.data[0];
	data[1] = m.data[1];
	data[2] = m.data[2];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T>& Mat3<T>::operator+=(const Mat3<T>& m)
{
	data[0] += m.data[0];
	data[1] += m.data[1];
	data[2] += m.data[2];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T>& Mat3<T>::operator-=(const Mat3<T>& m)
{
	data[0] -= m.data[0];
	data[1] -= m.data[1];
	data[2] -= m.data[2];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T>& Mat3<T>::operator*=(const Mat3<T>& m)
{
	data[0] *= m.data[0];
	data[1] *= m.data[1];
	data[2] *= m.data[2];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T>& Mat3<T>::operator/=(const Mat3<T>& m)
{
	data[0] /= m.data[0];
	data[1] /= m.data[1];
	data[2] /= m.data[2];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T>& Mat3<T>::operator++()
{
	++data[0];
	++data[1];
	++data[2];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> Mat3<T>::operator++(int)
{
	return Mat3<T>(data[0] + T(1), data[1] + T(1), data[2] + T(1));
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T>& Mat3<T>::operator--()
{
	--data[0];
	--data[1];
	--data[2];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> Mat3<T>::operator--(int)
{
	return Mat3<T>(data[0] - T(1), data[1] - T(1), data[2] - T(1));
}

// Binary + operator
template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator+(const Mat3<T>& m, T s)
{
	return Mat3<T>(m[0] + s, m[1] + s, m[2] + s);
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator+(T s, const Mat3<T>& m)
{
	return Mat3<T>(s + m[0], s + m[1], s + m[2]);
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator+(const Mat3<T>& m1, const Mat3<T>& m2)
{
	return Mat3<T>(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2]);
}

// Binary - operator
template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator-(const Mat3<T>& m, T s)
{
	return Mat3<T>(m[0] - s, m[1] - s, m[2] - s);
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator-(T s, const Mat3<T>& m)
{
	return Mat3<T>(s - m[0], s - m[1], s - m[2]);
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator-(const Mat3<T>& m1, const Mat3<T>& m2)
{
	return Mat3<T>(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2]);
}

// Binary * operator
template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator*(const Mat3<T>& m, T s)
{
	return Mat3<T>(m[0] * s, m[1] * s, m[2] * s);
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator*(T s, const Mat3<T>& m)
{
	return Mat3<T>(s * m[0], s * m[1], s * m[2]);
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator*(const Mat3<T>& m1, const Mat3<T>& m2)
{
	// m1[0][0] m1[1][0] m1[2][0]
	// m1[0][1] m1[1][1] m1[2][1]
	// m1[0][2] m1[1][2] m1[2][2]
	// *
	// m2[0][0] m2[1][0] m2[2][0]
	// m2[0][1] m2[1][1] m2[2][1]
	// m2[0][2] m2[1][2] m2[2][2]
	return Mat3<T>(
		m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2],
		m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2],
		m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2],
		m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2],
		m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2],
		m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2],
		m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2],
		m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2],
		m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2]);
}

template <typename T>
HINATA_FORCE_INLINE Vec3<T> operator*(const Mat3<T>& m, const Vec3<T>& v)
{
	// m[0][0] m[1][0] m[2][0]
	// m[0][1] m[1][1] m[2][1]
	// m[0][2] m[1][2] m[2][2]
	// *
	// v[0]
	// v[1]
	// v[2]
	return Vec3<T>(
		m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
}

// Binary / operator
template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator/(const Mat3<T>& m, T s)
{
	return Mat3<T>(m[0] / s, m[1] / s, m[2] / s);
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator/(T s, const Mat3<T>& m)
{
	return Mat3<T>(s / m[0], s / m[1], s / m[2]);
}

template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator/(const Mat3<T>& m1, const Mat3<T>& m2)
{
	return Mat3<T>(m1[0] / m2[0], m1[1] / m2[1], m1[2] / m2[2]);
}

// Unary - operator
template <typename T>
HINATA_FORCE_INLINE Mat3<T> operator-(const Mat3<T>& m)
{
	return Mat3<T>(-m[0], -m[1], -m[2]);
}

// Boolean operators
template <typename T>
HINATA_FORCE_INLINE bool operator==(const Mat3<T>& m1, const Mat3<T>& m2)
{
	return m1[0] == m2[0] && m1[1] == m2[1] && m1[2] == m2[2];
}

template <typename T>
HINATA_FORCE_INLINE bool operator!=(const Mat3<T>& m1, const Mat3<T>& m2)
{
	return m1[0] != m2[0] || m1[1] != m2[1] || m1[2] != m2[2];
}

// --------------------------------------------------------------------------------

template <typename T>
HINATA_FORCE_INLINE Mat4<T>::Mat4()
{

}

template <typename T>
HINATA_FORCE_INLINE Mat4<T>::Mat4(const Mat4<T>& m)
{
	data[0] = m.data[0];
	data[1] = m.data[1];
	data[2] = m.data[2];
	data[3] = m.data[3];
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T>::Mat4(T v)
{
	data[0] = Vec4<T>(v);
	data[1] = Vec4<T>(v);
	data[2] = Vec4<T>(v);
	data[3] = Vec4<T>(v);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T>::Mat4(const Vec4<T>& v0, const Vec4<T>& v1, const Vec4<T>& v2, const Vec4<T>& v3)
{
	data[0] = v0;
	data[1] = v1;
	data[2] = v2;
	data[3] = v3;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T>::Mat4(
	T v00, T v10, T v20, T v30,
	T v01, T v11, T v21, T v31,
	T v02, T v12, T v22, T v32,
	T v03, T v13, T v23, T v33)
{
	data[0] = Vec4<T>(v00, v10, v20, v30);
	data[1] = Vec4<T>(v01, v11, v21, v31);
	data[2] = Vec4<T>(v02, v12, v22, v32);
	data[3] = Vec4<T>(v03, v13, v23, v33);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Mat4<T>::Zero()
{
	return Mat4<T>();
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Mat4<T>::Diag(T v)
{
	return Mat4<T>(
		Vec4<T>(v, T(0), T(0), T(0)),
		Vec4<T>(T(0), v, T(0), T(0)),
		Vec4<T>(T(0), T(0), v, T(0)),
		Vec4<T>(T(0), T(0), T(0), v));
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Mat4<T>::Identity()
{
	return Diag(T(1));
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T>& Mat4<T>::operator[](int i)
{
	return data[i];
}

template <typename T>
HINATA_FORCE_INLINE const Vec4<T>& Mat4<T>::operator[](int i) const
{
	return data[i];
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T>& Mat4<T>::operator=(const Mat4<T>& m)
{
	data[0] = m.data[0];
	data[1] = m.data[1];
	data[2] = m.data[2];
	data[3] = m.data[3];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T>& Mat4<T>::operator+=(const Mat4<T>& m)
{
	data[0] += m.data[0];
	data[1] += m.data[1];
	data[2] += m.data[2];
	data[3] += m.data[3];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T>& Mat4<T>::operator-=(const Mat4<T>& m)
{
	data[0] -= m.data[0];
	data[1] -= m.data[1];
	data[2] -= m.data[2];
	data[3] -= m.data[3];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T>& Mat4<T>::operator*=(const Mat4<T>& m)
{
	data[0] *= m.data[0];
	data[1] *= m.data[1];
	data[2] *= m.data[2];
	data[3] *= m.data[3];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T>& Mat4<T>::operator/=(const Mat4<T>& m)
{
	data[0] /= m.data[0];
	data[1] /= m.data[1];
	data[2] /= m.data[2];
	data[3] /= m.data[3];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T>& Mat4<T>::operator++()
{
	++data[0];
	++data[1];
	++data[2];
	++data[3];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Mat4<T>::operator++(int)
{
	return Mat4<T>(data[0] + T(1), data[1] + T(1), data[2] + T(1), data[3] + T(1));
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T>& Mat4<T>::operator--()
{
	--data[0];
	--data[1];
	--data[2];
	--data[3];
	return *this;
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> Mat4<T>::operator--(int)
{
	return Mat4<T>(data[0] - T(1), data[1] - T(1), data[2] - T(1), data[3] - T(1));
}

// Binary + operator
template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator+(const Mat4<T>& m, T s)
{
	return Mat4<T>(m[0] + s, m[1] + s, m[2] + s, m[3] + s);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator+(T s, const Mat4<T>& m)
{
	return Mat4<T>(s + m[0], s + m[1], s + m[2], s + m[3]);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator+(const Mat4<T>& m1, const Mat4<T>& m2)
{
	return Mat4<T>(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2], m1[3] + m2[3]);
}

// Binary - operator
template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator-(const Mat4<T>& m, T s)
{
	return Mat4<T>(m[0] - s, m[1] - s, m[2] - s, m[3] - s);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator-(T s, const Mat4<T>& m)
{
	return Mat4<T>(s - m[0], s - m[1], s - m[2], s - m[3]);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator-(const Mat4<T>& m1, const Mat4<T>& m2)
{
	return Mat4<T>(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2], m1[3] - m2[3]);
}

// Binary * operator
template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator*(const Mat4<T>& m, T s)
{
	return Mat4<T>(m[0] * s, m[1] * s, m[2] * s, m[3] * s);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator*(T s, const Mat4<T>& m)
{
	return Mat4<T>(s * m[0], s * m[1], s * m[2], s * m[3]);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator*(const Mat4<T>& m1, const Mat4<T>& m2)
{
	// m1[0][0] m1[1][0] m1[2][0] m1[3][0]
	// m1[0][1] m1[1][1] m1[2][1] m1[3][1]
	// m1[0][2] m1[1][2] m1[2][2] m1[3][2]
	// m1[0][3] m1[1][3] m1[2][3] m1[3][3]
	// *
	// m2[0][0] m2[1][0] m2[2][0] m2[3][0]
	// m2[0][1] m2[1][1] m2[2][1] m2[3][1]
	// m2[0][2] m2[1][2] m2[2][2] m2[3][2]
	// m2[0][3] m2[1][3] m2[2][3] m2[3][3]
	return Mat4<T>(
		m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3],
		m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3],
		m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3],
		m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2] + m1[3][3] * m2[0][3],
		m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3],
		m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3],
		m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3],
		m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2] + m1[3][3] * m2[1][3],
		m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3],
		m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3],
		m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2] + m1[3][2] * m2[2][3],
		m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1] + m1[2][3] * m2[2][2] + m1[3][3] * m2[2][3],
		m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2] + m1[3][0] * m2[3][3],
		m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2] + m1[3][1] * m2[3][3],
		m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1] + m1[2][2] * m2[3][2] + m1[3][2] * m2[3][3],
		m1[0][3] * m2[3][0] + m1[1][3] * m2[3][1] + m1[2][3] * m2[3][2] + m1[3][3] * m2[3][3]);
}

template <typename T>
HINATA_FORCE_INLINE Vec4<T> operator*(const Mat4<T>& m, const Vec4<T>& v)
{
	// m[0][0] m[1][0] m[2][0] m[3][0]
	// m[0][1] m[1][1] m[2][1] m[3][1]
	// m[0][2] m[1][2] m[2][2] m[3][2]
	// m[0][3] m[1][3] m[2][3] m[3][3]
	// *
	// v[0]
	// v[1]
	// v[2]
	// v[3]
	return Vec4<T>(
		m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w,
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w,
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w,
		m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w);
}

// Binary / operator
template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator/(const Mat4<T>& m, T s)
{
	return Mat4<T>(m[0] / s, m[1] / s, m[2] / s, m[3] / s);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator/(T s, const Mat4<T>& m)
{
	return Mat4<T>(s / m[0], s / m[1], s / m[2], s / m[3]);
}

template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator/(const Mat4<T>& m1, const Mat4<T>& m2)
{
	return Mat4<T>(m1[0] / m2[0], m1[1] / m2[1], m1[2] / m2[2], m1[3] / m2[3]);
}

// Unary - operator
template <typename T>
HINATA_FORCE_INLINE Mat4<T> operator-(const Mat4<T>& m)
{
	return Mat4<T>(-m[0], -m[1], -m[2], -m[3]);
}

// Boolean operators
template <typename T>
HINATA_FORCE_INLINE bool operator==(const Mat4<T>& m1, const Mat4<T>& m2)
{
	return m1[0] == m2[0] && m1[1] == m2[1] && m1[2] == m2[2] && m1[3] == m2[3];
}

template <typename T>
HINATA_FORCE_INLINE bool operator!=(const Mat4<T>& m1, const Mat4<T>& m2)
{
	return m1[0] != m2[0] || m1[1] != m2[1] || m1[2] != m2[2] || m1[3] != m2[3];
}

HINATA_NAMESPACE_END
