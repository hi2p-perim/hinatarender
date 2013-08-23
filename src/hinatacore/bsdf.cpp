#include "pch.h"
#include <hinatacore/bsdf.h>
#include <hinatacore/intersection.h>

HINATA_NAMESPACE_BEGIN

double BSDF::CosTheta( const Vec3d& v )
{
	return v.z;
}

double BSDF::SinTheta2( const Vec3d& v )
{
	return 1.0 - v.z * v.z;
}

double BSDF::TanTheta( const Vec3d& v )
{
	double t = 1 - v.z * v.z;
	return t <= 0 ? 0 : std::sqrt(t) / v.z;
}

Vec3d BSDF::Reflect( const Vec3d& wi )
{
	return Vec3d(-wi.x, -wi.y, wi.z);
}

Vec3d BSDF::Refract( const Vec3d& wi, double eta, double cosThetaT )
{
	return Vec3d(-eta * wi.x, -eta * wi.y, cosThetaT);
}

double BSDF::ShadingNormalCorrectionFactor( BSDFRecord& record, Intersection& isect )
{
	// Prevent light leak
	// In some cases wi and wo are same side according to the shading normal but
	// opposite side according to the geometry normal.
	auto worldWi = isect.shadingToWorld * record.wi;
	auto worldWo = isect.shadingToWorld * record.wo;
	double wiDotNg = Math::Dot(worldWi, isect.gn);
	double woDotNg = Math::Dot(worldWo, isect.gn);

	if (wiDotNg * CosTheta(record.wi) <= 0 || woDotNg * CosTheta(record.wo) <= 0)
	{
		return 0.0;
	}

	// Special handling for adjoint case
	// Be careful of the difference of the notation between Veach's thesis;
	// in the framework, wo is always the propagating direction.
	if (record.adjoint)
	{
		// |w_i, N_s| * |w_o, N_g| / |w_i, N_g| / |w_o, N_s| 
		return CosTheta(record.wi) * woDotNg / (CosTheta(record.wo) * wiDotNg);
	}

	return 1.0;
}

HINATA_NAMESPACE_END