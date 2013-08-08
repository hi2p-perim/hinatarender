#include "pch.h"
#include <hinatacore/renderutils.h>

HINATA_NAMESPACE_BEGIN

double RenderUtils::Luminance( const Vec3d& c )
{
	return 0.212671 * c.r + 0.715160 * c.g + 0.072169 * c.b;
}

hinata::Vec2d RenderUtils::ConcentricDiskSample( const Vec2d& u )
{
	// Uniform sampling on the square [-1, 1]^2
	double v1 = 2.0 * u.x - 1.0;
	double v2 = 2.0 * u.y - 1.0;

	// Convert (sx, sy) to (r, theta)
	Vec2d conv;

	if (v1 == 0.0 && v2 == 0.0)
	{
		conv = Vec2d(0);
	}
	else if (v1 > -v2)
	{
		if (v1 > v2)
			conv = Vec2d(v1, (Pi / 4.0) * v2/v1);
		else
			conv = Vec2d(v2, (Pi / 4.0) * (2.0 - v1/v2));
	}
	else
	{
		if (v1 < v2)
			conv = Vec2d(-v1, (Pi / 4.0) * (4.0 + v2/v1));
		else
			conv = Vec2d(-v2, (Pi / 4.0) * (6.0 - v1/v2));
	}

	return Vec2d(conv.x * std::cos(conv.y), conv.x * std::sin(conv.y));
}

hinata::Vec3d RenderUtils::CosineSampleHemisphere( const Vec2d& u )
{
	auto s = ConcentricDiskSample(u);
	return Vec3d(s, std::sqrt(std::max(0.0, 1.0 - s.x*s.x - s.y*s.y)));
}

hinata::Vec3d RenderUtils::UniformSampleHemisphere( const Vec2d& u )
{
	double z = u.x;
	double r = std::sqrt(std::max(0.0, 1.0 - z*z));
	double phi = 2.0 * Pi * u.y;
	return Vec3d(r * std::cos(phi), r * std::sin(phi), z);
}

hinata::Vec3d RenderUtils::UniformSampleSphere( const Vec2d& u )
{
	double z = 1.0 - 2.0 * u.x;
	double r = std::sqrt(std::max(0.0, 1.0 - z*z));
	double phi = 2.0 * Pi * u.y;
	return Vec3d(r * std::cos(phi), r * std::sin(phi), z);
}

hinata::Vec2d RenderUtils::UniformSampleTriangle( const Vec2d& u )
{
	double s = std::sqrt(std::max(0.0, u.x));
	return Vec2d(1.0 - s, u.y * s);
}

void RenderUtils::CreateCoordinateSystem( const Vec3d& a, Vec3d& b, Vec3d& c )
{
	c = std::abs(a.x) > std::abs(a.y) ? Normalize(Vec3d(a.z, 0, -a.x)) : Normalize(Vec3d(0, a.z, -a.y));
	b = Normalize(Cross(c, a));
}

HINATA_NAMESPACE_END