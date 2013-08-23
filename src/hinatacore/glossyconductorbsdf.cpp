#include "pch.h"
#include <hinatacore/glossyconductorbsdf.h>

HINATA_NAMESPACE_BEGIN

GlossyConductorBSDF::GlossyConductorBSDF( const Vec3d& R, const Vec3d& eta, const Vec3d& k, double roughness )
	: R(R)
	, eta(eta)
	, k(k)
	, roughness(roughness)
{

}

Vec3d GlossyConductorBSDF::Evaluate( BSDFRecord& record, Intersection& isect )
{
	if ((record.type & BSDFType::GlossyReflection) == 0 || CosTheta(record.wi) <= 0 || CosTheta(record.wo) <= 0)
	{
		return Vec3d();
	}

	// Correction factor for shading normal
	double sf = ShadingNormalCorrectionFactor(record, isect);
	if (sf == 0.0)
	{
		return Vec3d();
	}

	// Half vector
	auto H = Math::Normalize(record.wi + record.wo);

	// Microfacet normal distribution
	double D = EvalBechmannDist(H);

	// Shadow masking function
	double G = EvalG(record.wi, record.wo, H);

	// Fresnel term
	auto F = EvalFrConductor(Math::Dot(record.wi, H));

	// f(wi, wo) * cos(wo)
	// = R * D * G * F / (4.0 * cos(wi) * cos(wo)) * cos(wo)
	// = R * D * G * F / (4.0 * cos(wi))
	return R * D * G * F / (4.0 * CosTheta(record.wi)) * sf;
}

Vec3d GlossyConductorBSDF::SampleAndEvaluate( BSDFRecord& record, BSDFSample& sample, double& pdf, Intersection& isect )
{
	if ((record.type & BSDFType::GlossyReflection) == 0 || CosTheta(record.wi) <= 0)
	{
		return Vec3d();
	}

	// Sample the microfacet normal H
	auto H = SampleBechmannDist(sample.u, pdf);
	if (pdf == 0.0)
	{
		return Vec3d();
	}

	// Reflect according to H
	// Reflect(I, N) = I - N * (N, I) * 2
	record.wo = -record.wi - Math::Dot(record.wi, H) * record.wi * 2.0;
	record.sampledType = BSDFType::GlossyReflection;

	if (CosTheta(record.wo) <= 0)
	{
		pdf = 0.0;
		return Vec3d();
	}

	// Correction factor for shading normal
	double sf = ShadingNormalCorrectionFactor(record, isect);
	if (sf == 0.0)
	{
		pdf = 0.0;
		return Vec3d();
	}

	// Fresnel term
	auto F = EvalFrConductor(Math::Dot(record.wi, H));

	// Microfacet normal distribution
	double D = EvalBechmannDist(H);

	// Shadow masking function
	double G = EvalG(record.wi, record.wo, H);

	double n = D * G * Math::Dot(record.wi, H);
	double d = pdf * CosTheta(record.wi);

	pdf /= 4.0 * Math::Dot(record.wo, H);

	return R * F * n / d * sf;
}

double GlossyConductorBSDF::Pdf( BSDFRecord& record )
{
	if ((record.type & BSDFType::GlossyReflection) == 0 || CosTheta(record.wi) <= 0 || CosTheta(record.wo) <= 0)
	{
		return 0.0;
	}

	// Half vector
	auto H = Math::Normalize(record.wi + record.wo);

	return EvalBechmannDist(H) * CosTheta(H) / (4.0 * Math::Abs(Math::Dot(record.wo, H)));
}

double GlossyConductorBSDF::EvalBechmannDist( const Vec3d& H )
{
	if (CosTheta(H) <= 0)
	{
		return 0.0;
	}

	double ex = TanTheta(H) / roughness;
	return std::exp(-(ex * ex)) / (Pi * roughness * roughness * std::pow(CosTheta(H), 4.0));
}

Vec3d GlossyConductorBSDF::SampleBechmannDist( const Vec2d& u, double& pdf )
{
	double tanThetaHSqr = -roughness * roughness * std::log(1.0 - u.x);
	double cosThetaH = 1.0 / std::sqrt(1.0 + tanThetaHSqr);
	double cosThetaH2 = cosThetaH * cosThetaH;
	double cosThetaH3 = cosThetaH2 * cosThetaH;

	pdf = (1.0 - u.x) / (Pi * roughness * roughness * cosThetaH3);

	double sinThetaH = std::sqrt(std::max(0.0, 1.0 - cosThetaH2));
	double phiH = 2.0 * Pi * u.y;

	return Vec3d(sinThetaH * std::cos(phiH), sinThetaH * std::sin(phiH), cosThetaH);
}

double GlossyConductorBSDF::EvalG( const Vec3d& wi, const Vec3d& wo, const Vec3d& H )
{
	// Assume cos(..) is positive

	double n_dot_H = CosTheta(H);
	double n_dot_wo = CosTheta(wo);
	double n_dot_wi = CosTheta(wi);
	double wo_dot_H = std::abs(Math::Dot(wo, H));
	double wi_dot_H = std::abs(Math::Dot(wo, H));
	return
		std::min(
			1.0,
			std::min(
				2.0 * n_dot_H * n_dot_wo / wo_dot_H,
				2.0 * n_dot_H * n_dot_wi / wi_dot_H));
}

Vec3d GlossyConductorBSDF::EvalFrConductor( double cosThetaI )
{
	auto tmp = (eta*eta + k*k) * (cosThetaI * cosThetaI);
	auto rParl2 =
		(tmp - (eta * (2.0 * cosThetaI)) + Vec3d(1)) /
		(tmp + (eta * (2.0 * cosThetaI)) + Vec3d(1));

	auto tmpF = eta*eta + k*k;
	auto rPerp2 =
		(tmpF - (eta * (2.0 * cosThetaI)) + Vec3d(cosThetaI*cosThetaI)) /
		(tmpF + (eta * (2.0 * cosThetaI)) + Vec3d(cosThetaI*cosThetaI));

	return (rParl2 + rPerp2) * 0.5;
}

HINATA_NAMESPACE_END