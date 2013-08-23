#include "pch.h"
#include <hinatacore/sphere.h>
#include <hinatacore/ray.h>
#include <hinatacore/intersection.h>
#include <hinatacore/renderutils.h>

HINATA_NAMESPACE_BEGIN

Sphere::Sphere( double radius, const Vec3d& position )
	: radius(radius)
	, position(position)
{

}

bool Sphere::Intersect( Ray& ray, Intersection& isect )
{
	auto po = ray.o - position;
	double a = Math::Dot(ray.d, ray.d);
	double b = 2.0 * Math::Dot(po, ray.d);
	double c = Math::Dot(po, po) - radius * radius;
	double det = b * b - 4.0 * a * c;

	if (det < 0.0f)
	{
		return false;
	}

	double t;
	double e = std::sqrt(det);
	double denom = 2.0 * a;
	double t0 = (-b - e) / denom;
	double t1 = (-b + e) / denom;

	if (t0 > ray.maxT || t1 < ray.minT)
	{
		return false;
	}

	t = t0;
	if (t < ray.minT)
	{
		t = t1;
		if (t > ray.maxT)
		{
			return false;
		}
	}

	isect.p = ray.o + t * ray.d;
	isect.gn = isect.sn = Math::Normalize(isect.p - position);

	RenderUtils::CreateCoordinateSystem(isect.sn, isect.ss, isect.st);

	isect.uv = Vec2d();
	isect.rayEpsilon = 1e-5 * t;
	ray.maxT = t;

	return true;
}

void Sphere::SamplePosition( ShapePositionSampleRecord& record, const Mat4d& transform )
{
	if (transform != Mat4d(1.0))
	{
		throw std::exception("Not supported");
	}

	auto v = RenderUtils::UniformSampleSphere(record.sample);

	record.p = position + v * radius;
	record.n = v;
	record.pdf = 1.0 / LocalArea();
}

double Sphere::Area( const Mat4d& transform )
{
	if (transform != Mat4d(1.0))
	{
		throw std::exception("Not suported");
	}

	return LocalArea();
}

AABB Sphere::Bound( const Mat4d& transform )
{
	if (transform != Mat4d(1.0))
	{
		throw std::exception("Not suported");
	}

	return LocalBound();
}

AABB Sphere::LocalBound()
{
	return AABB(Vec3d(-radius), Vec3d(radius));
}

double Sphere::LocalArea()
{
	return 4.0 * Pi * radius * radius;
}

HINATA_NAMESPACE_END