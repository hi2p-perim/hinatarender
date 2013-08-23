#include "pch.h"
#include <hinatacore/arealight.h>
#include <hinatacore/primitive.h>
#include <hinatacore/shape.h>
#include <hinatacore/renderutils.h>

HINATA_NAMESPACE_BEGIN

AreaLight::AreaLight( const Vec3d& L )
	: L(L)
{

}

void AreaLight::AddPrimitive( const std::shared_ptr<Primitive>& primitive )
{
	primitives.push_back(primitive);
}

void AreaLight::Initialize()
{
	// Create CDF
	primitiveAreaCdf.clear();
	primitiveAreaCdf.push_back(0);

	for (auto& primitive : primitives)
	{
		primitiveAreaCdf.push_back(primitiveAreaCdf.back() + primitive->Area());
	}

	// Normalize
	area = primitiveAreaCdf.back();
	for (double& v : primitiveAreaCdf)
	{
		v /= area;
	}

	power = L * Pi * area;
}

Vec3d AreaLight::SampleAndEvaluate( SampleRecord& sampleRecord )
{
	Vec2d ps(sampleRecord.positionSample);

	// Choose a shape according to the area
	int idx =
		Math::Clamp(
		(int)(std::upper_bound(primitiveAreaCdf.begin(), primitiveAreaCdf.end(), ps.y) - primitiveAreaCdf.begin() - 1),
		0, (int)primitiveAreaCdf.size() - 2);

	// Reuse sample
	ps.y = (ps.y - primitiveAreaCdf[idx]) / (primitiveAreaCdf[idx+1] - primitiveAreaCdf[idx]);

	// Sample position
	// Note: pdf is not used (eliminated in transformation)
	ShapePositionSampleRecord psr;
	psr.sample = ps;
	primitives[idx]->SamplePosition(psr);

	// Sample direction
	auto localDir = RenderUtils::CosineSampleHemisphere(sampleRecord.directionSample);

	Vec3d s, t;
	RenderUtils::CreateCoordinateSystem(psr.n, s, t);

	auto localToWorld = Mat3d(s, t, psr.n);

	sampleRecord.d = localToWorld * localDir;
	sampleRecord.p = psr.p;
	sampleRecord.n = psr.n;
	sampleRecord.pdf = PdfPosition() * PdfDirection(sampleRecord.d, sampleRecord.n);

	// Return value is
	//  \frac{L_e(x_0\to x_1)G(x_0\leftrightarrow x_1)}{p_A(x_0)p_A(x_1)}
	//  = power
	// where
	//  p_A(x_1)=p_{\sigma^\bot}(x_0\to x_1)G(x_0\leftrightarrow x_1),
	//  p_{\sigma^\bot}(x_0\to x_1)=p_{\sigma}(x_0\to x_1)/\cos{(\theta_0)},
	// and
	//  L_e(x_0\to x_1) = L,
	//  p_A(x_0) = 1 / area,
	//  p_{\sigma}(x_0\to x_1) = \cos{(\theta_0)}/\pi,
	//  power = L * area * \pi
	return power;
}

void AreaLight::SamplePosition( SampleRecord& sampleRecord )
{
	Vec2d ps(sampleRecord.positionSample);

	// Choose a shape according to the area
	int idx =
		Math::Clamp(
		(int)(std::upper_bound(primitiveAreaCdf.begin(), primitiveAreaCdf.end(), ps.y) - primitiveAreaCdf.begin() - 1),
		0, (int)primitiveAreaCdf.size() - 2);

	// Reuse sample
	ps.y = (ps.y - primitiveAreaCdf[idx]) / (primitiveAreaCdf[idx+1] - primitiveAreaCdf[idx]);

	// Sample position
	ShapePositionSampleRecord psr;
	psr.sample = ps;
	primitives[idx]->SamplePosition(psr);

	sampleRecord.p = psr.p;
	sampleRecord.n = psr.n;
	sampleRecord.pdf = PdfPosition();
}

Vec3d AreaLight::EvaluateCos( const Vec3d& d, const Vec3d& n )
{
	double cosTheta = Math::Dot(d, n);

	if (cosTheta < 0.0)
	{
		return Vec3d();
	}

	// L_e(x_0\to x_1) \cos{(\theta_0)} / p_A(x_0)
	// = L * area * \cos{(\theta_0)}
	return L * area * cosTheta;
}

Vec3d AreaLight::Evaluate( const Vec3d& d, const Vec3d& n )
{
	return
		Math::Dot(d, n) < 0.0
			? Vec3d()
			: L;
}

double AreaLight::PdfDirection( const Vec3d& d, const Vec3d& n )
{
	double cosTheta = Math::Dot(d, n);
	return
		cosTheta < 0.0
			? 0.0
			: cosTheta * InvPi;
}

HINATA_NAMESPACE_END