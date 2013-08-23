#include "pch.h"
#include <hinatacore/diffusebsdf.h>
#include <hinatacore/intersection.h>
#include <hinatacore/texture.h>
#include <hinatacore/renderutils.h>

HINATA_NAMESPACE_BEGIN

DiffuseBSDF::DiffuseBSDF( const Vec3d& R )
{
	this->R = std::make_shared<ConstantTexture>(R);
}

DiffuseBSDF::DiffuseBSDF( const std::shared_ptr<Texture>& R )
	: R(R)
{

}

Vec3d DiffuseBSDF::Evaluate( BSDFRecord& record, Intersection& isect )
{
	if ((record.type & BSDFType::DiffuseReflection) == 0 || CosTheta(record.wi) <= 0 || CosTheta(record.wo) <= 0)
	{
		return Vec3d();
	}

	// Correction factor
	double sf = ShadingNormalCorrectionFactor(record, isect);
	if (sf == 0.0)
	{
		return Vec3d();
	}

	return R->Evaluate(isect.uv) * InvPi * CosTheta(record.wo) * sf;
}

Vec3d DiffuseBSDF::SampleAndEvaluate( BSDFRecord& record, BSDFSample& sample, double& pdf, Intersection& isect )
{
	if ((record.type & BSDFType::DiffuseReflection) == 0 || CosTheta(record.wi) <= 0)
	{
		return Vec3d();
	}

	// Sample direction (cosine weighted)
	record.wo = RenderUtils::CosineSampleHemisphere(sample.u);
	record.sampledType = BSDFType::DiffuseReflection;

	// Pdf
	pdf = Pdf(record);
	if (pdf == 0.0)
	{
		return Vec3d();
	}

	// Correction factor for shading normal
	double sf = ShadingNormalCorrectionFactor(record, isect);
	if (sf == 0.0)
	{
		return Vec3d();
	}

	// f(wi, wo) * cos(theta) / p(wo)
	// = R * invPi * cos(theta) / (cos(theta) * invPi) = R
	return R->Evaluate(isect.uv) * sf;
}

double DiffuseBSDF::Pdf( BSDFRecord& record )
{
	if ((record.type & BSDFType::DiffuseReflection) == 0 || CosTheta(record.wi) <= 0 || CosTheta(record.wo) <= 0)
	{
		return 0.0;
	}

	return CosTheta(record.wo) * InvPi;
}

HINATA_NAMESPACE_END