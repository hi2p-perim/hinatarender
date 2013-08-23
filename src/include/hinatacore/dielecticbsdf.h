#ifndef __HINATA_CORE_DIELECTIC_BSDF_H__
#define __HINATA_CORE_DIELECTIC_BSDF_H__

#include "bsdf.h"

HINATA_NAMESPACE_BEGIN

class DielecticBSDF : public BSDF
{
public:

	DielecticBSDF(const Vec3d& R, const Vec3d& T, double n1, double n2);
	BSDFType Type() { return BSDFType::Delta; }
	Vec3d Evaluate(BSDFRecord& record, Intersection& isect);
	Vec3d SampleAndEvaluate(BSDFRecord& record, BSDFSample& sample, double& pdf, Intersection& isect);
	double Pdf(BSDFRecord& record);

private:

	// Evaluate Fresnel reflectance for dielectrics
	double EvalFrDielectic(double etaI, double etaT, double cosThetaI, double& cosThetaT);

private:

	Vec3d R;	// Specular reflectance
	Vec3d T;	// Specular transmittance
	double n1;		// External IOR
	double n2;		// Internal IOR

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_DIELECTIC_BSDF_H__