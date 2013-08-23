#include "pch.h"
#include <hinatacore/perspectivecamera.h>
#include <hinatacore/ray.h>

HINATA_NAMESPACE_BEGIN

PerspectiveCamera::PerspectiveCamera( const Mat4d& viewMatrix, const Mat4d& projectionMatrix )
	: viewMatrix(viewMatrix)
	, projectionMatrix(projectionMatrix)
{
	invProjectionMatrix = Math::Inverse(projectionMatrix);
	invViewMatrix = Math::Inverse(viewMatrix);

	// Position of the camera (in world coordinates)
	position = Vec3d(invViewMatrix * Vec4d(Vec3d(), 1.0));

	// Calculate area of the sensor used for SampleAndEvaluate
	auto ndcP1 = Vec3d(-1.0, -1.0, 0.0);
	auto ndcP2 = Vec3d(1.0, 1.0, 0.0);

	auto invProjectionMatrix = Math::Inverse(projectionMatrix);
	auto camP1_4 = invProjectionMatrix * Vec4d(ndcP1, 1.0);
	auto camP2_4 = invProjectionMatrix * Vec4d(ndcP2, 1.0);

	auto camP1 = Vec3d(camP1_4) / camP1_4.w;
	auto camP2 = Vec3d(camP2_4) / camP1_4.w;

	camP1 /= Vec3d(camP1.z);
	camP2 /= Vec3d(camP2.z);

	double A = (camP2.x - camP1.x) * (camP2.y - camP1.y);
	invA = 1.0 / A;
}

Vec3d PerspectiveCamera::SampleAndEvaluate( const Vec2d& pixelSample, Ray& ray, double& pdf )
{
	// Raster position in [-1, 1]^2
	auto ndcRasterPos = Vec3d(pixelSample * 2.0 - 1.0, 0.0);

	// Convert raster position to camera coordinates
	auto dirTCam4 = invProjectionMatrix * Vec4d(ndcRasterPos, 1.0);
	auto dirTCam3 = Math::Normalize(Vec3d(dirTCam4) / dirTCam4.w);

	ray.d = Math::Normalize(Vec3d(invViewMatrix * Vec4d(dirTCam3, 0.0)));
	ray.o = position;
	ray.minT = 0.0;
	ray.maxT = Inf;

	pdf = EvaluateImportance(-dirTCam3.z);
	assert(pdf != 0.0);

	// Returns
	//  W_e(z_0\to z_1) / p_A(z_0) / p_\sigma(z_0\to z_1)
	//  = 1
	// where
	//  W_e(z_0\to z_1) = p_\sigma(z_0\to z_1),
	//  p_A(z_0) = 1
	return Vec3d(1.0);
}

Vec3d PerspectiveCamera::SampleAndEvaluate( const Vec3d& ref, Vec2d& rasterPos, double& pdf )
{
	// Reference point in camera coordinates
	auto refCam4 = viewMatrix * Vec4d(ref, 1.0);
	auto refCam3 = Vec3d(refCam4);

	// Reference point in NDC
	auto refNdc4 = projectionMatrix * refCam4;
	auto refNdc3 = Vec3d(refNdc4) / refNdc4.w;

	// Raster position in [0, 1]^2
	rasterPos = (Vec2d(refNdc3.x, refNdc3.y) + 1.0) * 0.5;

	// Check visibility
	if (rasterPos.x < 0 || rasterPos.x > 1 || rasterPos.y < 0 || rasterPos.y > 1)
	{
		pdf = 0.0;
		return Vec3d();
	}

	// Importance
	double We = EvaluateImportance(-Math::Normalize(refCam3).z);
	if (We == 0.0)
	{
		pdf = 0.0;
		return Vec3d();
	}

	// Distance to the reference point to camera origin
	double dist2 = Math::Length2(refCam3);

	pdf = 1.0;
	return Vec3d(We / dist2);
}

double PerspectiveCamera::EvaluateImportance( double cosTheta )
{
	// Assume hypothetical sensor on z=-d in camera coordinates.
	// Then the sensitivity is 1/Ad^2 where A is area of the sensor when d=1.
	// Converting the measure, the sensitivity is
	//  W_e(z_0\to y_{s-1})
	//   = dA/d\omega 1/Ad^2
	//   = \| p - z_0 \|^2 / \cos{(\theta)} / Ad^2
	//   = 1 / (A * \cos^3{(\theta)}),
	// where p is the raster position on the sensor,
	// \theta is the angle between the normal on p and p - z_0.

	if (cosTheta <= 0.0)
	{
		// p is on behind the camera
		return 0.0;
	}

	double invCosTheta = 1.0 / cosTheta;
	return invA * invCosTheta * invCosTheta * invCosTheta;
}

HINATA_NAMESPACE_END