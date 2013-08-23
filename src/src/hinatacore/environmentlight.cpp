#include "pch.h"
#include <hinatacore/environmentlight.h>
#include <hinatacore/image.h>

HINATA_NAMESPACE_BEGIN

ConstantEnvironmentLight::ConstantEnvironmentLight( const Vec3d& Le )
	: Le(Le)
{
	
}

// ------------------------------------------------------------------------------------------

BitmapEnvironmentLight::BitmapEnvironmentLight( const std::string& path, double offset, double scale )
	: offset(offset)
	, scale(scale)
{
	image = std::make_shared<Image>(path, true);
}

Vec3d BitmapEnvironmentLight::Evaluate( const Vec3d& d )
{
	// Convert the ray direction to the latitude-longitude coordinates.
	auto rd = -d;
	Vec2d uv(
		Math::Fract(std::atan2(rd.x, -rd.z) * InvTwoPi + offset),
		std::acos(Math::Clamp(rd.y, -1.0, 1.0)) * InvPi);
	return image->Evaluate(uv) * scale;
}

HINATA_NAMESPACE_END