#ifndef __HINATA_CORE_BVH_SCENE_H__
#define __HINATA_CORE_BVH_SCENE_H__

#include "scene.h"
#include "math.h"
#include <memory>
#include <vector>

HINATA_NAMESPACE_BEGIN

struct BVHNode;
struct BVHBuildData;
struct BVHTraversalData;
class BSDF;
class AreaLight;
struct TriangleMesh;
class Primitive;

class BVHScene : public Scene
{
public:

	BVHScene(const std::string& scenePath);

public:

	bool Intersect(Ray& ray, Intersection& isect);
	std::vector<std::shared_ptr<Primitive>> Primitives() { return primitives; }

private:

	bool Intersect(const std::shared_ptr<BVHNode>& node, BVHTraversalData& data, Intersection& isect);
	bool Intersect(const AABB& bound, BVHTraversalData& data);
	std::shared_ptr<BVHNode> Build(const BVHBuildData& data, int begin, int end);
	void LoadPrimitives(const std::string& scenePath);

private:

	std::vector<std::tuple<std::shared_ptr<BSDF>, std::shared_ptr<AreaLight>>> materials;
	std::vector<std::shared_ptr<TriangleMesh>> meshes;
	std::vector<std::shared_ptr<Primitive>> primitives;

	int maxPrimitivesInNode;
	std::vector<int> bvhPrimitiveIndices;
	std::shared_ptr<BVHNode> root;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_BVH_SCENE_H__