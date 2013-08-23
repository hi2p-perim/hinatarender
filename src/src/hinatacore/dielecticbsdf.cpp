#include "pch.h"
#include <hinatacore/dielecticbsdf.h>

HINATA_NAMESPACE_BEGIN

DielecticBSDF::DielecticBSDF( const Vec3d& R, const Vec3d& T, double n1, double n2 )
	: R(R)
	, T(T)
	, n1(n1)
	, n2(n2)
{

}

Vec3d DielecticBSDF::Evaluate( BSDFRecord& record, Intersection& isect )
{
	bool useR = (record.type & BSDFType::DeltaReflection) != 0;
	bool useT = (record.type & BSDFType::DeltaTransmission) != 0;

	if (!useR && !useT)
	{
		return Vec3d();
	}

	double cosThetaI = CosTheta(record.wi);
	double cosThetaT = CosTheta(record.wo);
	bool entering = cosThetaI > 0.0;

	// Index of refraction
	double etaI = n1;
	double etaT = n2;

	if (!entering)
	{
		std::swap(etaI, etaT);
	}

	double eta = etaI / etaT;

	// Fresnel term
	double cosThetaT2;
	double Fr = EvalFrDielectic(etaI, etaT, cosThetaI, cosThetaT2);

	if (cosThetaI * cosThetaT >= 0)
	{
		// Reflection
		// Reflected wi and wo must be same
		if (!useR || Math::Dot(Reflect(record.wi), record.wo) < 1.0 - EpsLarge)
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
		return R * (Fr * sf);
	}
	else
	{
		// Refraction
		// Refracted wi and wo must be same
		if (!useT || Math::Dot(Refract(record.wi, eta, cosThetaT2), record.wo) < 1 - EpsLarge)
		{
			return Vec3d();
		}

		// Correction factor for shading normal
		double sf = ShadingNormalCorrectionFactor(record, isect);
		if (sf == 0.0)
		{
			return Vec3d();
		}

		// Correction factor for transmission
		double tf = !record.adjoint ? eta : 1.0;

		// Evaluation
		// Non-adjoint case
		// f(wi, wo) * cos(theta)
		// = (1/eta)^2 * T * (1-Fr) / cos(theta) * cos(theta)
		// = (1/eta)^2 * T * (1-Fr)
		// Adjoint case
		// f(wi, wo) * cos(theta) * eta^2
		// = T * (1-Fr)
		return T * ((1 - Fr) * tf * tf * sf);
	}

	return Vec3d();
}

Vec3d DielecticBSDF::SampleAndEvaluate( BSDFRecord& record, BSDFSample& sample, double& pdf, Intersection& isect )
{
	if ((record.type & BSDFType::DeltaReflection) == 0 || (record.type & BSDFType::DeltaTransmission) == 0)
	{
		pdf = 0.0;
		return Vec3d();
	}

	double cosThetaI = CosTheta(record.wi);
	bool entering = cosThetaI > 0.0;

	// Index of refraction
	double etaI = n1;
	double etaT = n2;

	if (!entering)
	{
		std::swap(etaI, etaT);
	}

	double eta = etaI / etaT;

	// Fresnel term
	double cosThetaT;
	double Fr = EvalFrDielectic(etaI, etaT, cosThetaI, cosThetaT);

	// Choose reflection or transmission using RR
	if (sample.uComponent <= Fr)
	{
		// Reflection
		record.wo = Reflect(record.wi);
		record.sampledType = BSDFType::DeltaReflection;
		pdf = Fr;

		// Correction factor for shading normal
		double sf = ShadingNormalCorrectionFactor(record, isect);
		if (sf == 0.0)
		{
			pdf = 0.0;
			return Vec3d();
		}

		// f(wi, wo) * cos(theta) / p(wo)
		// = R * Fr / cos(theta) * cos(theta) / Fr
		// = R
		return R * sf;
	}
	else
	{
		// Transmission
		record.wo = Refract(record.wi, eta, cosThetaT);
		record.sampledType = BSDFType::DeltaTransmission;
		pdf = 1.0 - Fr;

		// Correction factor for shading normal
		double sf = ShadingNormalCorrectionFactor(record, isect);
		if (sf == 0.0)
		{
			pdf = 0.0;
			return Vec3d();
		}

		// Correction factor for transmission
		double tf = !record.adjoint ? eta : 1.0;

		// Evaluation
		// Non-adjoint case
		// f(wi, wo) * cos(theta) / p(wo)
		// = (1/eta)^2 * T * (1-Fr) / cos(theta) * cos(theta) / (1-Fr)
		// = (1/eta)^2 * T
		// Adjoint case
		// f(wi, wo) * cos(theta) / p(wo) * eta^2
		// = T
		return  T * (tf * tf * sf);
	}

	// Not reached here
	return Vec3d();
}

double DielecticBSDF::Pdf( BSDFRecord& record )
{
	bool useR = (record.type & BSDFType::DeltaReflection) != 0;
	bool useT = (record.type & BSDFType::DeltaTransmission) != 0;

	if (!useR && !useT)
	{
		return 0.0;
	}

	double cosThetaI = CosTheta(record.wi);
	double cosThetaT = CosTheta(record.wo);
	bool entering = cosThetaI > 0.0;

	// Index of refraction
	double etaI = n1;
	double etaT = n2;

	if (!entering)
	{
		std::swap(etaI, etaT);
	}

	double eta = etaI / etaT;

	// Fresnel term
	double cosThetaT2;
	double Fr = EvalFrDielectic(etaI, etaT, cosThetaI, cosThetaT2);

	if (cosThetaI * cosThetaT >= 0)
	{
		// Reflection
		// Reflected wi and wo must be same
		if (!useR || Math::Dot(Reflect(record.wi), record.wo) < 1.0 - EpsLarge)
		{
			return 0.0;
		}

		return Fr;
	}
	else
	{
		// Refraction
		// Refracted wi and wo must be same
		if (!useT || Math::Dot(Refract(record.wi, eta, cosThetaT2), record.wo) < 1.0 - EpsLarge)
		{
			return 0.0;
		}

		return 1.0 - Fr;
	}

	return 0.0;
}

double DielecticBSDF::EvalFrDielectic( double etaI, double etaT, double cosThetaI, double& cosThetaT )
{
	double Fr;
	bool entering = cosThetaI > 0.0;

	// Using Snell's law, calculate sin^2(thetaT)
	double eta = etaI / etaT;
	double sinThetaTSq = eta * eta * (1.0 - cosThetaI * cosThetaI);

	if (sinThetaTSq >= 1.0)
	{
		// Total internal reflection
		Fr = 1.0; 
		cosThetaT = 0.0;
	}
	else
	{
		cosThetaI = abs(cosThetaI);
		cosThetaT = sqrt(1.0 - sinThetaTSq);

		if (etaI == etaT)
		{
			Fr = 0.0;
		}
		else
		{
			double Rs = (etaI * cosThetaI - etaT * cosThetaT) / (etaI * cosThetaI + etaT * cosThetaT);
			double Rp = (etaT * cosThetaI - etaI * cosThetaT) / (etaT * cosThetaI + etaI * cosThetaT);

			Fr = (Rs * Rs + Rp * Rp) * 0.5;

			// Flip theta_t if incoming ray comes from negative z
			if (entering)
			{
				cosThetaT = -cosThetaT;
			}
		}
	}

	return Fr;
}

HINATA_NAMESPACE_END