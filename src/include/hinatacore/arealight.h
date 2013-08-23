#ifndef __HINATA_CORE_AREA_LIGHT_H__
#define __HINATA_CORE_AREA_LIGHT_H__

#include "common.h"
#include "math.h"
#include <memory>

HINATA_NAMESPACE_BEGIN

class Primitive;

/*!
	Area light.
	Diffuse area light emitter.
*/
class AreaLight
{
public:

	/*!
		Sample record.
		The structure is used for evaluation or sample queries.
	*/
	struct SampleRecord
	{
		Vec2d positionSample;
		Vec2d directionSample;
		Vec3d p;
		Vec3d d;
		Vec3d n;
		double pdf;
	};

public:

	/*!
		Constructor.
		\param L Radiance.
	*/
	AreaLight(const Vec3d& L);

private:

	AreaLight(const AreaLight&);
	AreaLight(AreaLight&&);
	void operator=(const AreaLight&);
	void operator=(AreaLight&&);

public:

	/*!
		Add a primitive to the light.
		Multiple primitives can be added to a light.
		After the registration, Initialize function must be called.
		\param primitive Primitive to be added.
		\sa Initialize
	*/
	void AddPrimitive(const std::shared_ptr<Primitive>& primitive);

	/*!
		Initialize the light.
		The function must be called after registration of primitives
		by AddPrimitive function.
		\sa AddPrimitive
	*/
	void Initialize();

	/*!
		Sample and evaluate the light.
		Sample a position and a direction on the light, i.e.,
		- p_A(x_0) : A position in the light source (area measure)
		- p_\sigma(x_0\to x_1) : Direction emitted from the sampled position (solid angle measure)
		Evaluates
		- \frac{L_e(x_0\to x_1)G(x_0\leftrightarrow x_1)}{p_A(x_0)p_A(x_1)}
		Input
		- positionSample, directionSample
		Output
		- p, d, n
		\param sampleRecord Query record.
		\return Evaluated contribution.
	*/
	Vec3d SampleAndEvaluate(SampleRecord& sampleRecord);

	/*!
		Sample a position on the light.
		Input
		- positionSample
		Output
		- p, n, pdf
		\param sampleRecord Query record.
	*/
	void SamplePosition(SampleRecord& sampleRecord);

	/*!
		Evaluate cosine weighted contribution.
		Evaluate L_e(x_0\to x_1) \cos{(\theta_0)} / p_A(x_0)
		\return Evaluated contribution.
	*/
	Vec3d EvaluateCos(const Vec3d& d, const Vec3d& n);

	/*!
		Evaluate contribution.
		Evaluate L_e(x_{n-1}\to x_n).
		\param d Outgoing direction on the light surface.
		\param n Geometry normal.
		\return Evaluated contribution.
	*/
	Vec3d Evaluate(const Vec3d& d, const Vec3d& n);

	/*!
		Evaluate power.
		Evaluate power of the light.
		\return Power.
	*/
	Vec3d Power() { return power; }

	/*!
		Evaluate PDF of positional component.
		Evaluate p_A(x_n).
		\return Evaluated value.
	*/
	double PdfPosition() { return 1.0 / area; }

	/*!
		Evaluate PDF of directional component.
		Evaluate p_\sigma(y_0\to y_1).
		\param d Outgoing direction on the light surface.
		\param n Geometry normal.
		\return Evaluated value.
	*/
	double PdfDirection(const Vec3d& d, const Vec3d& n);

private:

	Vec3d L;
	std::vector<std::shared_ptr<Primitive>> primitives;
	std::vector<double> primitiveAreaCdf;
	double area;
	Vec3d power;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_AREA_LIGHT_H__