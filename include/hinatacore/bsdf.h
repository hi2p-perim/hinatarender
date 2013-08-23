#ifndef __HINATA_CORE_BSDF_H__
#define __HINATA_CORE_BSDF_H__

#include "common.h"
#include "math.h"

HINATA_NAMESPACE_BEGIN

/*!
	BSDF sample.
	Samples needed for BSDF sampling.
*/
struct BSDFSample
{
	Vec2d u;
	double uComponent;
};

/*!
	BSDF record.
	IO structure for BSDF.
*/
struct BSDFRecord
{
	int type;			//<! Requested BSDF type flag
	int sampledType;	//<! Sampled BSDF type
	double adjoint;		//<! Adjoint BSDF
	Vec3d wi;			//<! Input direction in local coordinates
	Vec3d wo;			//<! Output direction in local coordinates
};

///! BSDF types.
enum BSDFType
{
	// Primitive BSDF types
	DiffuseReflection		= 1<<0,
	DiffuseTransmission		= 1<<1,
	DeltaReflection			= 1<<2,
	DeltaTransmission		= 1<<3,
	GlossyReflection		= 1<<4,
	GlossyTransmission		= 1<<5,

	// Useful flags
	Diffuse					= DiffuseReflection | DiffuseTransmission,
	Delta					= DeltaReflection | DeltaTransmission,
	Glossy					= GlossyReflection | GlossyTransmission,
	All						= Diffuse | Delta | Glossy,
	Reflection				= DiffuseReflection | DeltaReflection | GlossyReflection,
	Transmission			= DiffuseTransmission | DeltaTransmission | GlossyTransmission,
};

class Intersection;

class BSDF
{
public:

	BSDF() {}
	virtual ~BSDF() {}

private:

	BSDF(const BSDF&);
	BSDF(BSDF&&);
	void operator=(const BSDF&);
	void operator=(BSDF&&);

public:

	/*!
		Get BSDF type.
		\return BSDF type.
	*/
	virtual BSDFType Type() = 0;

	/*!
		Evaluate BSDF.
		Evaluate f(wi, wo) * cos(theta).
	*/
	virtual Vec3d Evaluate(BSDFRecord& record, Intersection& isect) = 0;

	/*!
		Sample and evaluate BSDF.
		Sample wo and evalueate f(wi, wo) * cos(theta) / p(wo).
	*/
	virtual Vec3d SampleAndEvaluate(BSDFRecord& record, BSDFSample& sample, double& pdf, Intersection& isect) = 0;

	/*!
		Evaluate PDF.
		Evaluate PDF in solid angle measure.
	*/
	virtual double Pdf(BSDFRecord& record) = 0;

protected:

	// Useful operation on local shading coordinates
	double CosTheta(const Vec3d& v);
	double SinTheta2(const Vec3d& v);
	double TanTheta(const Vec3d& v);
	Vec3d Reflect(const Vec3d& wi);
	Vec3d Refract(const Vec3d& wi, double eta, double cosThetaT);
	double ShadingNormalCorrectionFactor(BSDFRecord& record, Intersection& isect);

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_BSDF_H__