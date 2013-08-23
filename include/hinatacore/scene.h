#ifndef __HINATA_CORE_SCENE_H__
#define __HINATA_CORE_SCENE_H__

#include "common.h"
#include "math.h"
#include <memory>

HINATA_NAMESPACE_BEGIN

class Ray;
class Intersection;
class PerspectiveCamera;
class AreaLight;
class EnvironmentLight;

class Scene
{
public:

	Scene() {}
	virtual ~Scene() {}

private:

	Scene(const Scene&);
	Scene(Scene&&);
	void operator=(const Scene&);
	void operator=(Scene&&);

public:

	/*!
		Intersection query.
		The function checks if the ray hits the scene.
		When intersected, information on the hit point
		is stored in the intersection data.
		\param ray Ray.
		\param isect Intersection data.
		\retval true Intersected with the scene.
		\retval false Not intersected with the scene.
	*/
	virtual bool Intersect(Ray& ray, Intersection& isect) = 0;

	/*!
		Get camera.
		Get main camera of the scene.
		We note that only one camera is allowed to exist.
	*/
	std::shared_ptr<PerspectiveCamera> Camera() { return camera; }

	/*!
		Sample light sources.
		We Note that given sample can be reused.
	*/
	void SampleLight(double& u, std::shared_ptr<AreaLight>& light, double& pdf);

	/*!
		Evaluate light selection PDF.
		Discrete PDF of selecting a light from the scene.
		\return Evaluated value.
	*/
	double LightSelectionPdf()
	{
		return 1.0 / (double)lights.size();
	}

	/*!
		Get environment light.
		\return Environment light.
	*/
	std::shared_ptr<EnvironmentLight> GetEnvironmentLight() { return environmentLight; }

protected:

	std::shared_ptr<PerspectiveCamera> camera;
	std::vector<std::shared_ptr<AreaLight>> lights;
	std::shared_ptr<EnvironmentLight> environmentLight;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_SCENE_H__