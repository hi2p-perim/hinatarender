#include "pch.h"
#include <hinatacore/triangle.h>
#include <hinatacore/ray.h>
#include <hinatacore/intersection.h>
#include <hinatacore/renderutils.h>

HINATA_NAMESPACE_BEGIN

Triangle::Triangle( const std::shared_ptr<TriangleMesh>& mesh, int v1, int v2, int v3 )
	: mesh(mesh)
	, v1(v1)
	, v2(v2)
	, v3(v3)
{

}

bool Triangle::Intersect( Ray& ray, Intersection& isect )
{
	auto& p1 = mesh->positions[v1];
	auto& p2 = mesh->positions[v2];
	auto& p3 = mesh->positions[v3];

	auto e1 = p2 - p1;
	auto e2 = p3 - p1;
	auto gn = Math::Normalize(Math::Cross(e1, e2));

	// If the intersected mesh is one sided, ignore the ray from the back side.
	if (mesh->oneSided && Math::Dot(gn, -ray.d) < 0)
	{
		return false;
	}

	auto s1 = Math::Cross(ray.d, e2);
	double divisor = Math::Dot(s1, e1);

	if (divisor == 0.0)
	{
		return false;
	}

	double invDivisor = 1.0 / divisor;

	// First barycentric coordinate
	auto d = ray.o - p1;
	double b1 = Math::Dot(d, s1) * invDivisor;

	if (b1 < 0.0 || b1 > 1.0)
	{
		return false;
	}

	// Second barycentric coordinate
	auto s2 = Math::Cross(d, e1);
	double b2 = Math::Dot(ray.d, s2) * invDivisor;

	if (b2 < 0.0 || b1 + b2 > 1.0)
	{
		return false;
	}

	// Intersection point
	double t = Math::Dot(e2, s2) * invDivisor;

	if (t < ray.minT || t > ray.maxT)
	{
		return false;
	}

	// ------------------------------------------------------------

	// Use shading normal
	auto& n1 = mesh->normals[v1];
	auto& n2 = mesh->normals[v2];
	auto& n3 = mesh->normals[v3];

	isect.p = ray.o + t * ray.d;
	isect.gn = gn;
	isect.sn = Math::Normalize(n1 * (1.0 - b1 - b2) + n2 * b1 + n3 * b2);

	RenderUtils::CreateCoordinateSystem(isect.sn, isect.ss, isect.st);

	// Texture coordinates
	if (!mesh->texcoords.empty())
	{
		auto& uv1 = mesh->texcoords[v1];
		auto& uv2 = mesh->texcoords[v2];
		auto& uv3 = mesh->texcoords[v3];

		isect.uv = uv1 * (1.0 - b1 - b2) + uv2 * b1 + uv3 * b2;
	}
	else
	{
		isect.uv = Vec2d();
	}

	isect.rayEpsilon = 1e-5 * t;
	ray.maxT = t;

	return true;
}

void Triangle::SamplePosition( ShapePositionSampleRecord& record, const Mat4d& transform )
{
	// Sample triangle
	auto b = RenderUtils::UniformSampleTriangle(record.sample);

	// Transformed positions
	auto p1 = Vec3d(transform * Vec4d(mesh->positions[v1], 1.0));
	auto p2 = Vec3d(transform * Vec4d(mesh->positions[v2], 1.0));
	auto p3 = Vec3d(transform * Vec4d(mesh->positions[v3], 1.0));

	// Sampled position
	record.p = p1 * (1.0 - b.x - b.y) + p2 * b.x + p3 * b.y;

	record.n = Math::Normalize(Math::Cross(p2 - p1, p3 - p1));

	//// Transformed normals
	//auto normalTransform = glm::mat3(glm::transpose(glm::inverse(transform)));

	//auto n1 = normalTransform * mesh->normals[v1];
	//auto n2 = normalTransform * mesh->normals[v2];
	//auto n3 = normalTransform * mesh->normals[v3];

	//// Sampled normal
	//record.n = glm::normalize(n1 * (1.0 - b.x - b.y) + n2 * b.x + n3 * b.y);

	record.pdf = 1 / Area(transform);
}

double Triangle::Area( const Mat4d& transform )
{
	auto p1 = Vec3d(transform * Vec4d(mesh->positions[v1], 1.0));
	auto p2 = Vec3d(transform * Vec4d(mesh->positions[v2], 1.0));
	auto p3 = Vec3d(transform * Vec4d(mesh->positions[v3], 1.0));

	return 0.5 * Math::Length(Math::Cross(p2 - p1, p3 - p1));
}

AABB Triangle::Bound( const Mat4d& transform )
{
	auto p1 = Vec3d(transform * Vec4d(mesh->positions[v1], 1.0));
	auto p2 = Vec3d(transform * Vec4d(mesh->positions[v2], 1.0));
	auto p3 = Vec3d(transform * Vec4d(mesh->positions[v3], 1.0));

	return AABB(p1, p2).Union(p3);
}

Vec3d Triangle::Position( int i, const Mat4d& transform )
{
	return
		i == 0 ? Vec3d(transform * Vec4d(mesh->positions[v1], 1.0)) :
		i == 1 ? Vec3d(transform * Vec4d(mesh->positions[v2], 1.0)) :
		i == 2 ? Vec3d(transform * Vec4d(mesh->positions[v3], 1.0)) : Vec3d();
}

HINATA_NAMESPACE_END