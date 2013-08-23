#include "pch.h"
#include <hinatacore/aabb.h>
#include <hinatacore/mathconsts.h>
#include <hinatacore/mathfuncs.h>

HINATA_NAMESPACE_BEGIN

AABB::AABB()
	: min(Inf)
	, max(-Inf)
{

}

AABB::AABB( const Vec3d& p )
	: min(p)
	, max(p)
{

}

AABB::AABB( const Vec3d& p1, const Vec3d& p2 )
	: min(Math::Min(p1, p2))
	, max(Math::Max(p1, p2))
{

}

bool AABB::Intersect( const AABB& b ) const
{
	bool x = (max.x >= b.min.x) && (min.x <= b.max.x);
	bool y = (max.y >= b.min.y) && (min.y <= b.max.y);
	bool z = (max.z >= b.min.z) && (min.z <= b.max.z);
	return x && y && z;
}

bool AABB::Contain( const Vec3d& p ) const
{
	return
		p.x >= min.x && p.x <= max.x &&
		p.y >= min.y && p.y <= max.y &&
		p.z >= min.z && p.z <= max.z;
}

double AABB::SurfaceArea() const
{
	Vec3d d = max - min;
	return 2.0 * (d.x * d.y + d.y * d.z + d.z * d.x);
}

double AABB::Volume() const
{
	Vec3d d = max - min;
	return d.x * d.y * d.z;
}

int AABB::LongestAxis() const
{
	Vec3d d = max - min;
	return d.x > d.y && d.x > d.z ? 0 : d.y > d.z ? 1 : 2;
}

AABB AABB::Union( const AABB& b )
{
	AABB r;
	r.min = Math::Min(min, b.min);
	r.max = Math::Max(max, b.max);
	return r;
}

AABB AABB::Union( const Vec3d& p )
{
	AABB r;
	r.min = Math::Min(min, p);
	r.max = Math::Max(max, p);
	return r;
}

bool AABB::operator==( const AABB& b ) const
{
	return min == b.min && max == b.max;
}

bool AABB::operator!=( const AABB& b ) const
{
	return min != b.min || max != b.max;
}

const Vec3d& AABB::operator[]( int i ) const
{
	return (&min)[i];
}

Vec3d& AABB::operator[]( int i )
{
	return (&min)[i];
}

HINATA_NAMESPACE_END