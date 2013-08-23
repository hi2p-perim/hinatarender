#ifndef __HINATA_CORE_GLOSSY_CONDUCTOR_BSDF_H__
#define __HINATA_CORE_GLOSSY_CONDUCTOR_BSDF_H__

#include "bsdf.h"

HINATA_NAMESPACE_BEGIN

class GlossyConductorBSDF : public BSDF
{
public:

	GlossyConductorBSDF(const Vec3d& R, const Vec3d& eta, const Vec3d& k, double roughness);

public:

	BSDFType Type() { return BSDFType::GlossyReflection; }
	Vec3d Evaluate(BSDFRecord& record, Intersection& isect);
	Vec3d SampleAndEvaluate(BSDFRecord& record, BSDFSample& sample, double& pdf, Intersection& isect);
	double Pdf(BSDFRecord& record);

private:

	// Evaluate Bechmann distribution
	double EvalBechmannDist(const Vec3d& H);

	// Sample Bechmann distribution
	Vec3d SampleBechmannDist(const Vec2d& u, double& pdf);

	// Evaluate shadow masking function
	double EvalG(const Vec3d& wi, const Vec3d& wo, const Vec3d& H);
	
	// Evaluate Fresnel reflectance for conductors
	Vec3d EvalFrConductor(double cosThetaI);

private:

	Vec3d R;
	Vec3d eta;
	Vec3d k;		
	double roughness;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_GLOSSY_CONDUCTOR_BSDF_H__