#ifndef __HINATA_CORE_INTERSECTION_H__
#define __HINATA_CORE_INTERSECTION_H__

#include "common.h"
#include "math.h"
#include <memory>

HINATA_NAMESPACE_BEGIN

class Primitive;

class Intersection
{
public:

	std::shared_ptr<Primitive> primitive;

	Vec3d p;		// Intersection point
	Vec3d gn;		// Geometry normal
	Vec3d sn;		// Shading normal
	Vec3d ss, st;	// Tangent vectors w.r.t. shading normal
	Vec2d uv;		// Texture coordinates

	//Vec3d dpdu, dpdv;
	//Vec3d dpdn, dpdv;

	double rayEpsilon;
	Mat3d worldToShading;
	Mat3d shadingToWorld;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_INTERSECTION_H__