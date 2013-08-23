#ifndef __HINATA_CORE_SHAPE_H__
#define __HINATA_CORE_SHAPE_H__

#include "common.h"
#include "math.h"
#include "aabb.h"

HINATA_NAMESPACE_BEGIN

class Ray;
class Intersection;

struct ShapePositionSampleRecord
{
	Vec2d sample;
	Vec3d p;
	Vec3d n;
	double pdf; // Area measure
};

class Shape
{
public:

	Shape() {}
	virtual ~Shape() {}

private:

	Shape(const Shape&);
	Shape(Shape&&);
	void operator=(const Shape&);
	void operator=(Shape&&);

public:

	virtual bool Intersect(Ray& ray, Intersection& isect) = 0;
	virtual void SamplePosition(ShapePositionSampleRecord& record, const Mat4d& transform) = 0;
	virtual double Area(const Mat4d& transform) = 0;
	virtual AABB Bound(const Mat4d& transform) = 0;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_SHAPE_H__