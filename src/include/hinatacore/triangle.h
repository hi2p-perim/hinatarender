#ifndef __HINATA_CORE_TRIANGLE_H__
#define __HINATA_CORE_TRIANGLE_H__

#include "shape.h"
#include <memory>

HINATA_NAMESPACE_BEGIN

struct TriangleMesh
{
	std::vector<Vec3d> positions;
	std::vector<Vec3d> normals;
	std::vector<Vec2d> texcoords;
	//std::vector<int> indices;
	bool oneSided;
};

class Triangle : public Shape
{
public:

	Triangle(const std::shared_ptr<TriangleMesh>& mesh, int v1, int v2, int v3);

public:

	bool Intersect( Ray& ray, Intersection& isect );
	void SamplePosition(ShapePositionSampleRecord& record, const Mat4d& transform);
	double Area(const Mat4d& transform);
	AABB Bound(const Mat4d& transform);
	Vec3d Position(int i, const Mat4d& transform);

private:

	std::shared_ptr<TriangleMesh> mesh;
	int v1, v2, v3;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_TRIANGLE_H__