#ifndef __HINATA_CORE_RENDER_UTIL_H__
#define __HINATA_CORE_RENDER_UTIL_H__

#include "common.h"
#include "math.h"

HINATA_NAMESPACE_BEGIN

class RenderUtils
{
private:

	RenderUtils() {}
	RenderUtils(const RenderUtils&);
	RenderUtils(const RenderUtils&&);
	RenderUtils& operator=(const RenderUtils&);
	RenderUtils& operator=(const RenderUtils&&);

public:

	static double Luminance(const Vec3d& c);
	static Vec2d ConcentricDiskSample(const Vec2d& u);
	static Vec3d CosineSampleHemisphere(const Vec2d& u);
	static Vec3d UniformSampleHemisphere(const Vec2d& u);
	static Vec3d UniformSampleSphere(const Vec2d& u);
	static Vec2d UniformSampleTriangle(const Vec2d& u);
	static void CreateCoordinateSystem(const Vec3d& a, Vec3d& b, Vec3d& c);

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_RENDER_UTIL_H__
