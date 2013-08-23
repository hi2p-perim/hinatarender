#ifndef __HINATA_CORE_PERFECT_MIRROR_BSDF_H__
#define __HINATA_CORE_PERFECT_MIRROR_BSDF_H__

#include "bsdf.h"

HINATA_NAMESPACE_BEGIN

class PerfectMirrorBSDF : public BSDF
{
public:

	PerfectMirrorBSDF(const Vec3d& R);

public:

	BSDFType Type() { return BSDFType::DeltaReflection; }
	Vec3d Evaluate(BSDFRecord& record, Intersection& isect);
	Vec3d SampleAndEvaluate(BSDFRecord& record, BSDFSample& sample, double& pdf, Intersection& isect);
	double Pdf(BSDFRecord& record);

private:

	Vec3d R;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_PERFECT_MIRROR_BSDF_H__