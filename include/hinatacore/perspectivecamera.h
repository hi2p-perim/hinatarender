#ifndef __HINATA_CORE_PERSPECTIVE_CAMERA_H__
#define __HINATA_CORE_PERSPECTIVE_CAMERA_H__

#include "common.h"
#include "math.h"

HINATA_NAMESPACE_BEGIN

class Ray;

class PerspectiveCamera
{
public:

	/*!
		Constructor.
		\param viewMatrix View matrix.
		\param projectionMatrix Perspective projection matrix.
	*/
	PerspectiveCamera(const Mat4d& viewMatrix, const Mat4d& projectionMatrix);

private:

	PerspectiveCamera(const PerspectiveCamera&);
	PerspectiveCamera(PerspectiveCamera&&);
	void operator=(const PerspectiveCamera&);
	void operator=(PerspectiveCamera&&);

public:

	/*!
		Sample the camera and evaluate the contribution.
		The function is used for sampling the initial ray.
		Sample z_0\to z_1.
		Evaluate W_e(z_0\to z_1) / p_A(z_0) / p_\sigma(z_0\to z_1).
		\param pixelSample 
	*/
	Vec3d SampleAndEvaluate(const Vec2d& pixelSample, Ray& ray, double& pdf);

	/*!
		Sample the camera and evaluate the contribution.
		The function is used for direct sampling of the camera.
		We note that the evaluated contribution contains
		inverse square distance term for preserving consistency.
		Sample z_0\in\mathcal{M}.
		Evaluate W_e(z_0\to y_{s-1}) / p_A(z_0) / \| y_{s-1} - z_0 \|^2.
	*/
	Vec3d SampleAndEvaluate(const Vec3d& ref, Vec2d& rasterPos, double& pdf);

	/*!
		Position of the camera.
		\return Position of the camera.
	*/
	Vec3d Position() { return position; }

private:

	/*!
		Evaluate importance.
		Evaluate W_e(z_0\to y_{s-1}).
		Importance can be considered as the sensitivity of the sensor.
	*/
	double EvaluateImportance(double cosTheta);

private:

	Mat4d viewMatrix;
	Mat4d projectionMatrix;
	Mat4d invViewMatrix;
	Mat4d invProjectionMatrix;

	Vec3d position;
	double invA;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_PERSPECTIVE_CAMERA_H__