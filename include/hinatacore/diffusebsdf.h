#ifndef __HINATA_CORE_DIFFUSE_BSDF_H__
#define __HINATA_CORE_DIFFUSE_BSDF_H__

#include "bsdf.h"
#include <memory>

HINATA_NAMESPACE_BEGIN

class Texture;

class DiffuseBSDF : public BSDF
{
public:

	DiffuseBSDF(const Vec3d& R);
	DiffuseBSDF(const std::shared_ptr<Texture>& R);

public:

	BSDFType Type() { return BSDFType::DiffuseReflection; }
	Vec3d Evaluate(BSDFRecord& record, Intersection& isect);
	Vec3d SampleAndEvaluate(BSDFRecord& record, BSDFSample& sample, double& pdf, Intersection& isect);
	double Pdf(BSDFRecord& record);
	
public:

	std::shared_ptr<Texture> R;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_DIFFUSE_BSDF_H__