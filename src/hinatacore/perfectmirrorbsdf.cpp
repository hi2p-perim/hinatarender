#include "pch.h"
#include <hinatacore/perfectmirrorbsdf.h>

HINATA_NAMESPACE_BEGIN

PerfectMirrorBSDF::PerfectMirrorBSDF( const Vec3d& R )
	: R(R)
{

}

Vec3d PerfectMirrorBSDF::Evaluate( BSDFRecord& record, Intersection& isect )
{
	if ((record.type & BSDFType::DeltaReflection) == 0 || CosTheta(record.wi) <= 0 || CosTheta(record.wo) <= 0)
	{
		return Vec3d();
	}

	if (Math::Dot(Reflect(record.wi), record.wo) < 1.0 - EpsLarge)
	{
		return Vec3d();
	}

	// Correction factor for shading normal
	double sf = ShadingNormalCorrectionFactor(record, isect);
	if (sf == 0.0)
	{
		return Vec3d();
	}

	// f(wi, wo) * cos(theta)
	// = R * Fr / cos(theta) * cos(theta)
	// = R * Fr
	// = R (omit Fresnel factor here)
	return R * sf;
}

Vec3d PerfectMirrorBSDF::SampleAndEvaluate( BSDFRecord& record, BSDFSample& sample, double& pdf, Intersection& isect )
{
	if ((record.type & BSDFType::DeltaReflection) == 0 || CosTheta(record.wi) <= 0)
	{
		return Vec3d();
	}

	record.wo = Reflect(record.wi);
	pdf = 1.0;

	// Correction factor for shading normal
	double sf = ShadingNormalCorrectionFactor(record, isect);
	if (sf == 0.0)
	{
		pdf = 0.0;
		return Vec3d();
	}

	// f(wi, wo) * cos(theta) / p(wo)
	// = R * Fr / cos(theta) * cos(theta) / p(wo)
	// = R * Fr
	// = R (omit Fresnel factor here)
	return R * sf;
}

double PerfectMirrorBSDF::Pdf( BSDFRecord& record )
{
	if ((record.type & BSDFType::DeltaReflection) == 0 || CosTheta(record.wi) <= 0 || CosTheta(record.wo) <= 0)
	{
		return 0.0;
	}

	if (Math::Dot(Reflect(record.wi), record.wo) < 1.0 - EpsLarge)
	{
		return 0.0;
	}

	return 1.0;
}

HINATA_NAMESPACE_END