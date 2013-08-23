#include "pch.h"
#include <hinatacore/primitive.h>
#include <hinatacore/shape.h>
#include <hinatacore/bsdf.h>
#include <hinatacore/ray.h>
#include <hinatacore/intersection.h>
#include <hinatacore/arealight.h>

HINATA_NAMESPACE_BEGIN

Primitive::Primitive( const Mat4d& localToWorld, const std::shared_ptr<Shape>& shape, const std::shared_ptr<BSDF>& bsdf )
	: localToWorld(localToWorld)
	, shape(shape)
	, bsdf(bsdf)
{
	InitializeTransform();
}

Primitive::Primitive( const Mat4d& localToWorld, const std::shared_ptr<Shape>& shape, const std::shared_ptr<BSDF>& bsdf, const std::shared_ptr<AreaLight>& light )
	: localToWorld(localToWorld)
	, shape(shape)
	, bsdf(bsdf)
	, light(light)
{
	InitializeTransform();
}

bool Primitive::Intersect( Ray& ray, Intersection& isect )
{
	Ray localRay(ray);

	localRay.o = Vec3d(worldToLocal * Vec4d(ray.o, 1.0));
	localRay.d = Vec3d(worldToLocal * Vec4d(ray.d, 0.0));

	if (!shape->Intersect(localRay, isect))
	{
		return false;
	}

	ray.minT = localRay.minT;
	ray.maxT = localRay.maxT;

	if (localToWorld != Mat4d(1.0))
	{
		isect.p = Vec3d(localToWorld * Vec4d(isect.p, 1.0));
		isect.sn = Math::Normalize(normalLocalToWorld * isect.sn);
		isect.gn = Math::Normalize(normalLocalToWorld * isect.gn);
		isect.ss = Math::Normalize(Vec3d(localToWorld * Vec4d(isect.ss, 0.0)));
		isect.st = Math::Normalize(Vec3d(localToWorld * Vec4d(isect.st, 0.0)));
	}

	return true;
}

void Primitive::SamplePosition( ShapePositionSampleRecord& record )
{
	shape->SamplePosition(record, localToWorld);
}

double Primitive::Area()
{
	return shape->Area(localToWorld);
}

AABB Primitive::Bound()
{
	return shape->Bound(localToWorld);
}

void Primitive::InitializeTransform()
{
	worldToLocal = Math::Inverse(localToWorld);
	normalLocalToWorld = Mat3d(Math::Transpose(worldToLocal));
}

HINATA_NAMESPACE_END