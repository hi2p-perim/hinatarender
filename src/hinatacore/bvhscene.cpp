#include "pch.h"
#include <hinatacore/bvhscene.h>
#include <hinatacore/ray.h>
#include <hinatacore/intersection.h>
#include <hinatacore/primitive.h>
#include <hinatacore/triangle.h>
#include <hinatacore/texture.h>
#include <hinatacore/diffusebsdf.h>
#include <hinatacore/perfectmirrorbsdf.h>
#include <hinatacore/dielecticbsdf.h>
#include <hinatacore/glossyconductorbsdf.h>
#include <hinatacore/arealight.h>
#include <hinatacore/environmentlight.h>
#include <hinatacore/perspectivecamera.h>
#include <hinatacore/scenedata.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

HINATA_NAMESPACE_BEGIN

struct BVHNode
{

	enum class NodeType
	{
		Leaf,
		Internal
	};

	BVHNode(int begin, int end, const AABB& bound)
		: type(NodeType::Leaf)
		, begin(begin)
		, end(end)
		, bound(bound)
	{

	}

	BVHNode(int splitAxis, const std::shared_ptr<BVHNode>& left, const std::shared_ptr<BVHNode>& right)
		: type(NodeType::Internal)
		, splitAxis(splitAxis)
		, left(left)
		, right(right)
	{
		bound = left->bound.Union(right->bound);
	}

	NodeType type;
	AABB bound;

	// Leaf node data
	// Primitives index in [begin, end)
	int begin, end;

	// Internal node data
	std::shared_ptr<BVHNode> left, right;
	int splitAxis;

};

struct BVHBuildData
{
	std::vector<AABB> primitiveBounds;				// Bounds of the primitives
	std::vector<Vec3d> primitiveBoundCentroids;		// Centroid of the bounds of the primitives
};

class CompareToBucket
{
public:

	CompareToBucket(int splitAxis, int numBuckets, int minCostIdx, const BVHBuildData& data, const AABB& centroidBound)
		: splitAxis(splitAxis)
		, numBuckets(numBuckets)
		, minCostIdx(minCostIdx)
		, data(data)
		, centroidBound(centroidBound)
	{}

	bool operator()(int i) const
	{
		int bucketIdx = 
			(int)((double)numBuckets * 
			((data.primitiveBoundCentroids[i][splitAxis] - centroidBound.min[splitAxis]) /
			(centroidBound.max[splitAxis] - centroidBound.min[splitAxis])));

		bucketIdx = Math::Min(bucketIdx, numBuckets - 1);
		return bucketIdx <= minCostIdx;
	}

private:

	int splitAxis;
	int numBuckets;
	int minCostIdx;
	const BVHBuildData& data;
	const AABB& centroidBound;

};

struct BVHTraversalData
{

	BVHTraversalData(Ray& ray)
		: ray(ray)
	{
		invRayDir = Vec3d(1.0 / ray.d.x, 1.0 / ray.d.y, 1.0 / ray.d.z);
		rayDirNegative = Vec3i(ray.d.x < 0.0, ray.d.y < 0.0, ray.d.z < 0.0);
	}

	Ray& ray;
	Vec3i rayDirNegative;	// Each component of the rayDir is negative
	Vec3d invRayDir;		// Inverse of the rayDir

};

// --------------------------------------------------------------------------------

BVHScene::BVHScene( const std::string& scenePath )
	: maxPrimitivesInNode(255)
{
	LoadPrimitives(scenePath);

	BVHBuildData data;

	for (size_t i = 0; i < primitives.size(); i++)
	{
		// Initial index
		bvhPrimitiveIndices.push_back((int)i);

		// Temporary data for building
		AABB& primitiveBound = primitives[i]->Bound();

		data.primitiveBounds.push_back(primitiveBound);
		data.primitiveBoundCentroids.push_back((primitiveBound.min + primitiveBound.max) * 0.5);
	}

	// Build BVH
	root = Build(data, 0, (int)primitives.size());
}

bool BVHScene::Intersect( Ray& ray, Intersection& isect )
{
	BVHTraversalData data(ray);

	if (Intersect(root, data, isect))
	{
		// Fill in the information in isect
		// Compute conversion to/from shading coordinates
		isect.worldToShading = Math::Transpose(Mat3d(isect.ss, isect.st, isect.sn));
		isect.shadingToWorld = Math::Inverse(isect.worldToShading);

		return true;
	}

	return false;
}

bool BVHScene::Intersect( const std::shared_ptr<BVHNode>& node, BVHTraversalData& data, Intersection& isect )
{
	bool intersected = false;

	// Check intersection to the node bound
	if (Intersect(node->bound, data))
	{
		if (node->type == BVHNode::NodeType::Leaf)
		{
			// Leaf node
			// Intersection with the primitives hold in the node
			for (int i = node->begin; i < node->end; i++)
			{
				std::shared_ptr<Primitive>& primitive = primitives[bvhPrimitiveIndices[i]];
				if (primitive->Intersect(data.ray, isect))
				{
					intersected = true;
					isect.primitive = primitive;
				}
			}
		}
		else
		{
			// Internal node
			// Traverse the right node first if the ray direction according to the
			// split axis is negative.
			if (data.rayDirNegative[node->splitAxis])
			{
				intersected |= Intersect(node->right, data, isect);
				intersected |= Intersect(node->left, data, isect);
			}
			else
			{
				intersected |= Intersect(node->left, data, isect);
				intersected |= Intersect(node->right, data, isect);
			}
		}
	}

	return intersected;
}

bool BVHScene::Intersect( const AABB& bound, BVHTraversalData& data )
{
	auto& rayDirNegative = data.rayDirNegative;
	auto& invRayDir = data.invRayDir;
	auto& ray = data.ray;

	double tmin =  (bound[    rayDirNegative[0]].x - ray.o.x) * invRayDir.x;
	double tmax =  (bound[1 - rayDirNegative[0]].x - ray.o.x) * invRayDir.x;
	double tymin = (bound[    rayDirNegative[1]].y - ray.o.y) * invRayDir.y;
	double tymax = (bound[1 - rayDirNegative[1]].y - ray.o.y) * invRayDir.y;

	if ((tmin > tymax) || (tymin > tmax)) return false;
	if (tymin > tmin) tmin = tymin;
	if (tymax < tmax) tmax = tymax;

	// Check for ray intersection against z slab
	double tzmin = (bound[    rayDirNegative[2]].z - ray.o.z) * invRayDir.z;
	double tzmax = (bound[1 - rayDirNegative[2]].z - ray.o.z) * invRayDir.z;

	if ((tmin > tzmax) || (tzmin > tmax)) return false;
	if (tzmin > tmin) tmin = tzmin;
	if (tzmax < tmax) tmax = tzmax;

	return (tmin < ray.maxT) && (tmax > ray.minT);
}

std::shared_ptr<BVHNode> BVHScene::Build( const BVHBuildData& data, int begin, int end )
{
	std::shared_ptr<BVHNode> node;	

	// Bound of the primitive [begin, end)
	AABB bound;

	for (int i = begin; i < end; i++)
	{
		bound = bound.Union(data.primitiveBounds[bvhPrimitiveIndices[i]]);
	}

	// Number of primitives in the node
	int numPrimitives = end - begin;

	if (numPrimitives == 1)
	{
		// Leaf node
		node = std::make_shared<BVHNode>(begin, end, bound);
	}
	else
	{
		// Internal node

		// Choose the axis to split
		AABB centroidBound;

		for (int i = begin; i < end; i++)
		{
			centroidBound = centroidBound.Union(data.primitiveBoundCentroids[bvhPrimitiveIndices[i]]);
		}

		int splitAxis = centroidBound.LongestAxis();

		// If the centroid bound according to the split axis
		// is degenerated, take the node as a leaf.
		if (centroidBound.min[splitAxis] == centroidBound.max[splitAxis])
		{
			node = std::make_shared<BVHNode>(begin, end, bound);
		}
		else
		{
			// Split primitives using SAH, surface area heuristic.

			// Considering all possible partitions is rather heavy in the computation cost,
			// so in the application the primitives is separated to some buckets according to the split axis
			// and reduce the combination of the partitions.

			const int numBuckets = 12;

			AABB bucketPrimitiveBound[numBuckets];
			int bucketPrimitiveCount[numBuckets] = {0};

			// Create buckets
			for (int i = begin; i < end; i++)
			{
				int bucketIdx =
					(int)((double)numBuckets * 
						((data.primitiveBoundCentroids[bvhPrimitiveIndices[i]][splitAxis] - centroidBound.min[splitAxis]) /
						(centroidBound.max[splitAxis] - centroidBound.min[splitAxis])));

				bucketIdx = Math::Min(bucketIdx, numBuckets - 1);

				bucketPrimitiveCount[bucketIdx]++;
				bucketPrimitiveBound[bucketIdx] = bucketPrimitiveBound[bucketIdx].Union(data.primitiveBounds[bvhPrimitiveIndices[i]]);
			}

			// Compute costs
			// Note that the number of possible partitions is numBuckets - 1.
			double costs[numBuckets - 1];

			// For each partition compute the cost
			for (int i = 0; i < numBuckets - 1; i++)
			{
				AABB b1;
				AABB b2;
				int count1 = 0;
				int count2 = 0;

				// [0, i]
				for (int j = 0; j <= i; j++)
				{
					b1 = b1.Union(bucketPrimitiveBound[j]);
					count1 += bucketPrimitiveCount[j];
				}

				// (i, numBuckets - 1]
				for (int j = i + 1; j < numBuckets; j++)
				{
					b2 = b2.Union(bucketPrimitiveBound[j]);
					count2 += bucketPrimitiveCount[j];
				}

				// Assume the intersection cost is 1 and traversal cost is 1/8.
				costs[i] = 0.125 + ((double)count1 * b1.SurfaceArea() + (double)count2 * b2.SurfaceArea()) / bound.SurfaceArea();
			}

			// Find minimum partition
			int minCostIdx = 0;
			double minCost = costs[0];

			for (int i = 1; i < numBuckets - 1; i++)
			{
				if (minCost > costs[i])
				{
					minCost = costs[i];
					minCostIdx = i;
				}
			}

			// Partition if the minimum cost is lower than the leaf cost (numPrimitives)
			// or the current number of primitives is higher than the limit.
			if (minCost < (double)numPrimitives || numPrimitives > maxPrimitivesInNode)
			{
				int mid = (int)(std::partition(&bvhPrimitiveIndices[begin],
					&bvhPrimitiveIndices[end - 1] + 1, CompareToBucket(splitAxis, numBuckets, minCostIdx, data, centroidBound))
					- &bvhPrimitiveIndices[0]);

				node = std::make_shared<BVHNode>(
					splitAxis,
					Build(data, begin, mid),
					Build(data, mid, end));
			}
			else
			{
				// Otherwise make leaf node
				node = std::make_shared<BVHNode>(begin, end, bound);
			}
		}
	}

	return node;
}

void BVHScene::LoadPrimitives( const std::string& scenePath )
{
	// Deserialize scene
	std::ifstream ifs(scenePath, std::ifstream::in | std::ifstream::binary);

	if (!ifs)
	{
		throw std::exception(boost::str(
			boost::format("std::ifstream : %s") % boost::filesystem::path(scenePath).string()).c_str());
	}

	boost::shared_ptr<SceneData> sceneData;

	try
	{
		boost::archive::binary_iarchive ia(ifs);
		ia & boost::serialization::make_nvp("HinataScene", sceneData);
	}
	catch (boost::archive::archive_exception& e)
	{
		throw std::exception(boost::str(
			boost::format("boost::archive::archive_exception : %s") % e.what()).c_str());
	}

	// --------------------------------------------------------------------------------

	// Meshes
	for (auto& meshData : sceneData->meshes)
	{
		auto mesh = std::make_shared<TriangleMesh>();

		// Move attributes
		mesh->positions = std::move(meshData->positions);
		mesh->normals = std::move(meshData->normals);
		mesh->texcoords = std::move(meshData->texcoords);

		// One-sided?
		mesh->oneSided = meshData->oneSided;

		meshes.push_back(mesh);
	}

	// Materials
	boost::unordered_map<std::string, std::shared_ptr<BitmapTexture>> texturePathMap;

	for (auto& materialData : sceneData->materials)
	{
		std::shared_ptr<BSDF> bsdf;
		std::shared_ptr<AreaLight> light;

		switch (materialData->type)
		{
			case SceneDataElement_MaterialType::DiffuseBSDF_Texture:
			{
				// Find path and if not found, load texture
				if (texturePathMap.find(materialData->texturePath) == texturePathMap.end())
				{
					texturePathMap[materialData->texturePath] = std::make_shared<BitmapTexture>(materialData->texturePath);
				}

				// Create BSDF
				bsdf = std::make_shared<DiffuseBSDF>(texturePathMap[materialData->texturePath]);
				break;
			}

			case SceneDataElement_MaterialType::DiffuseBSDF_Color:
			{
				bsdf = std::make_shared<DiffuseBSDF>(materialData->color);
				break;
			}

			case SceneDataElement_MaterialType::PerfectMirrorBSDF:
			{
				bsdf = std::make_shared<PerfectMirrorBSDF>(materialData->color);
				break;
			}

			case SceneDataElement_MaterialType::DielecticBSDF:
			{
				bsdf = std::make_shared<DielecticBSDF>(materialData->color, materialData->color, 1.0, materialData->ior);
				break;
			}

			case SceneDataElement_MaterialType::GlossyConductorBSDF:
			{
				bsdf = std::make_shared<GlossyConductorBSDF>(
					materialData->color,
					Vec3d(0.140000, 0.129000, 0.158500),
					Vec3d(4.586250, 3.348125, 2.329375),
					materialData->shininess / 512.0);
				break;
			}

			case SceneDataElement_MaterialType::Emissive:
			{
				bsdf = std::make_shared<DiffuseBSDF>(materialData->color);
				light = std::make_shared<AreaLight>(materialData->emissiveColor);
				lights.push_back(light);
				break;
			}
		}

		materials.push_back(std::make_tuple(bsdf, light));
	}

	// Camera
	camera = std::make_shared<PerspectiveCamera>(
		sceneData->camera.viewMatrix,
		sceneData->camera.projectionMatrix);

	// Primitives
	for (auto& primitiveData : sceneData->primitives)
	{
		for (int meshIndex : primitiveData->meshIndices)
		{
			auto& meshData = sceneData->meshes[meshIndex];
			auto& material = materials[meshData->materialIndex];

			auto& bsdf = std::get<0>(material);
			auto& light = std::get<1>(material);

			for (auto& face : meshData->faces)
			{
				// Shape
				auto triangle = std::make_shared<Triangle>(
					meshes[meshIndex], face[0], face[1], face[2]);

				// Primitive
				std::shared_ptr<Primitive> primitive;

				if (light != nullptr)
				{
					primitive = std::make_shared<Primitive>(primitiveData->transform, triangle, bsdf, light);
					light->AddPrimitive(primitive);
				}
				else
				{
					primitive = std::make_shared<Primitive>(primitiveData->transform, triangle, bsdf);
				}

				primitives.push_back(primitive);
			}
		}
	}

	// Initialize lights
	for (auto& material : materials)
	{
		auto& light = std::get<1>(material);

		if (light != nullptr)
		{
			light->Initialize();
		}
	}

	// Setup the environment light
	// If the path to the environment map is not specified, use constant environment light.
	auto& envMapData = sceneData->envMap;
	if (envMapData.path == "")
	{
		environmentLight = std::make_shared<ConstantEnvironmentLight>(envMapData.bgColor);
	}
	else
	{
		environmentLight = std::make_shared<BitmapEnvironmentLight>(envMapData.path, envMapData.offset, envMapData.scale);
	}
}

HINATA_NAMESPACE_END


