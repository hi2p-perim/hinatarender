#ifndef __HINATA_CORE_PRIMITIVE_H__
#define __HINATA_CORE_PRIMITIVE_H__

#include "common.h"
#include "math.h"
#include <memory>

HINATA_NAMESPACE_BEGIN

class Shape;
struct ShapePositionSampleRecord;
class BSDF;
class Ray;
class Intersection;
class PerspectiveCamera;
class AreaLight;

class Primitive
{
public:

	Primitive(const Mat4d& localToWorld, const std::shared_ptr<Shape>& shape, const std::shared_ptr<BSDF>& bsdf);
	Primitive(const Mat4d& localToWorld, const std::shared_ptr<Shape>& shape, const std::shared_ptr<BSDF>& bsdf, const std::shared_ptr<AreaLight>& light);

private:

	Primitive(const Primitive&);
	Primitive(Primitive&&);
	void operator=(const Primitive&);
	void operator=(Primitive&&);

public:

	bool Intersect(Ray& ray, Intersection& isect);
	void SamplePosition(ShapePositionSampleRecord& record);
	double Area();
	AABB Bound();

	std::shared_ptr<Shape> GetShape() { return shape; }
	std::shared_ptr<BSDF> Bsdf() { return bsdf; }
	std::shared_ptr<AreaLight> Light() { return light; }
	Mat4d LocalToWorld() { return localToWorld; }

private:

	void InitializeTransform();

private:

	Mat4d localToWorld;
	Mat4d worldToLocal;
	Mat3d normalLocalToWorld;

	std::shared_ptr<Shape> shape;
	std::shared_ptr<BSDF> bsdf;
	std::shared_ptr<AreaLight> light;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_PRIMITIVE_H__