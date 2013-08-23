#ifndef __HINATA_CORE_RAY_H__
#define __HINATA_CORE_RAY_H__

#include "common.h"

HINATA_NAMESPACE_BEGIN

class Ray
{
public:

	Vec3d o;
	Vec3d d;
	double maxT, minT;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_RAY_H__