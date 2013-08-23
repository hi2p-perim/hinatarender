#ifndef __HINATA_CORE_CORNELL_BOX_SCENE_H__
#define __HINATA_CORE_CORNELL_BOX_SCENE_H__

#include "scene.h"
#include <vector>

HINATA_NAMESPACE_BEGIN

class Ray;
class Intersection;
class Primitive;

/*!
	Cornell box scene.
	Hard coded Cornell box scene composed of spheres.
*/
class CornellBoxScene : public Scene
{
public:

	/*!
		Constructor.
		\param aspect Aspect ratio.
	*/
	CornellBoxScene(double aspect);

public:

	bool Intersect(Ray& ray, Intersection& isect);

private:

	std::vector<std::shared_ptr<Primitive>> primitives;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_CORNELL_BOX_SCENE_H__