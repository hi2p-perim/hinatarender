#ifndef __HINATA_CORE_AABB_H__
#define __HINATA_CORE_AABB_H__

#include "common.h"
#include "vector.h"

HINATA_NAMESPACE_BEGIN

/*!
	AABB.
	Axis-aligned bounding box.
*/
class AABB
{
public:

	AABB();
	AABB(const Vec3d& p);
	AABB(const Vec3d& p1, const Vec3d& p2);

public:

	bool Intersect(const AABB& b) const;
	bool Contain(const Vec3d& p) const;
	double SurfaceArea() const;
	double Volume() const;
	int LongestAxis() const;
	AABB Union(const AABB& b);
	AABB Union(const Vec3d& p);
	bool operator==(const AABB& b) const;
	bool operator!=(const AABB& b) const;
	const Vec3d& operator[](int i) const;
	Vec3d& operator[](int i);

public:

	Vec3d min, max;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_AABB_H__