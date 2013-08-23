#ifndef __HINATA_CORE_SPHERE_H__
#define __HINATA_CORE_SPHERE_H__

#include "shape.h"

HINATA_NAMESPACE_BEGIN

/*!
	Sphere.
	Note that transformed sphere is not supported.
	Specify identity transform in the primitive associated with the shape.
*/
class Sphere : public Shape
{
public:

	Sphere(double radius, const Vec3d& position);

public:

	bool Intersect(Ray& ray, Intersection& isect);
	void SamplePosition(ShapePositionSampleRecord& record, const Mat4d& transform);
	double Area(const Mat4d& transform);
	AABB Bound(const Mat4d& transform);

private:

	AABB LocalBound();
	double LocalArea();

private:

	double radius;
	Vec3d position;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_SPHERE_H__