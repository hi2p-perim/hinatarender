#include "pch.h"
#include <hinatacore/cornellboxscene.h>
#include <hinatacore/ray.h>
#include <hinatacore/intersection.h>
#include <hinatacore/sphere.h>
#include <hinatacore/triangle.h>
#include <hinatacore/diffusebsdf.h>
#include <hinatacore/dielecticbsdf.h>
#include <hinatacore/glossyconductorbsdf.h>
#include <hinatacore/arealight.h>
#include <hinatacore/primitive.h>
#include <hinatacore/perspectivecamera.h>

HINATA_NAMESPACE_BEGIN

CornellBoxScene::CornellBoxScene( double aspect )
{
	// Fixed scene definition (Cornell Box)

	// Right
	primitives.push_back(
		std::make_shared<Primitive>(
			Mat4d::Identity(),
			std::make_shared<Sphere>(1e5, Vec3d(1e5+1, 0, 0)),
			std::make_shared<DiffuseBSDF>(Vec3d(0.75, 0.25, 0.25))));

	// Left
	primitives.push_back(
		std::make_shared<Primitive>(
			Mat4d::Identity(),
			std::make_shared<Sphere>(1e5, Vec3d(-1e5-1, 0, 0)),
			std::make_shared<DiffuseBSDF>(Vec3d(0.25, 0.25, 0.75))));

	// Top
	primitives.push_back(
		std::make_shared<Primitive>(
			Mat4d::Identity(),
			std::make_shared<Sphere>(1e5, Vec3d(0, 1e5+1, 0)),
			std::make_shared<DiffuseBSDF>(Vec3d(0.75))));

	// Bottom
	primitives.push_back(
		std::make_shared<Primitive>(
			Mat4d::Identity(),
			std::make_shared<Sphere>(1e5, Vec3d(0, -1e5-1, 0)),
			std::make_shared<DiffuseBSDF>(Vec3d(0.75))));

	// Back
	primitives.push_back(
		std::make_shared<Primitive>(
			Mat4d::Identity(),
			std::make_shared<Sphere>(1e5, Vec3d(0, 0, 1e5+7)),
			std::make_shared<DiffuseBSDF>(Vec3d())));

	// Front
	primitives.push_back(
		std::make_shared<Primitive>(
			Mat4d::Identity(),
			std::make_shared<Sphere>(1e5, Vec3d(0, 0, -1e5-1)),
			std::make_shared<DiffuseBSDF>(Vec3d(0.75))));
	//std::make_shared<GlossyConductorBSDF>(Vec3d(1), Vec3d(0.1), Vec3d(1.67), 0.08)));

	// Light
	auto light = std::make_shared<AreaLight>(Vec3d(30));

	//auto lp =
	//	std::make_shared<Primitive>(
	//		Mat4d(1.0),
	//		std::make_shared<Sphere>(0.15, Vec3d(0.5, 0.7, 0)),
	//		//std::make_shared<Sphere>(0.15, Vec3d(0, 0.85, 0)),
	//		std::make_shared<DiffuseBSDF>(Vec3d(0.75)),
	//		light);

	auto mesh = std::make_shared<TriangleMesh>();
	mesh->positions.push_back(Vec3d( 0.2, 0.9,  0.2));
	mesh->positions.push_back(Vec3d(-0.2, 0.9,  0.2));
	mesh->positions.push_back(Vec3d(-0.2, 0.9, -0.2));
	mesh->positions.push_back(Vec3d( 0.2, 0.9, -0.2));
	mesh->normals.push_back(Vec3d(0, -1, 0));
	mesh->normals.push_back(Vec3d(0, -1, 0));
	mesh->normals.push_back(Vec3d(0, -1, 0));
	mesh->normals.push_back(Vec3d(0, -1, 0));

	auto lp1 =
		std::make_shared<Primitive>(
			Math::Rotate(30.0, Vec3d(0, 0, 1)),
			std::make_shared<Triangle>(mesh, 0, 1, 3),
			std::make_shared<DiffuseBSDF>(Vec3d(0.75)),
			//std::make_shared<DiffuseBSDF>(Vec3d()),
			light);

	primitives.push_back(lp1);
	light->AddPrimitive(lp1);

	auto lp2 =
		std::make_shared<Primitive>(
			Math::Rotate(30.0, Vec3d(0, 0, 1)),
			std::make_shared<Triangle>(mesh, 1, 2, 3),
			std::make_shared<DiffuseBSDF>(Vec3d(0.75)),
			//std::make_shared<DiffuseBSDF>(Vec3d()),
			light);

	primitives.push_back(lp2);
	light->AddPrimitive(lp2);

	light->Initialize();
	lights.push_back(light);

	// Ball 1
	primitives.push_back(
		std::make_shared<Primitive>(
			Mat4d(1.0),
			std::make_shared<Sphere>(0.35, Vec3d(0.45, -0.65, 0.5)),
			//std::make_shared<DiffuseBSDF>(Vec3d(0.75))));
			std::make_shared<DielecticBSDF>(Vec3d(1), Vec3d(1), 1, 1.544)));

	// Ball 2
	primitives.push_back(
		std::make_shared<Primitive>(
			Mat4d(1.0),
			std::make_shared<Sphere>(0.35, Vec3d(-0.45, -0.65, -0.3)),
			std::make_shared<DiffuseBSDF>(Vec3d(0.75))));
			//std::make_shared<GlossyConductorBSDF>(Vec3d(1), Vec3d(0.1), Vec3d(1.67), 0.05)));

	// Camera
	camera = std::make_shared<PerspectiveCamera>(
		Math::LookAt(Vec3d(0, 0, 6.99), Vec3d(0, 0, 0), Vec3d(0, 1, 0)),
		Math::Perspective(20.0, aspect, 0.1, 1000.0));
}

bool CornellBoxScene::Intersect( Ray& ray, Intersection& isect )
{
	bool intersected = false;

	for (auto& primitive : primitives)
	{
		if (primitive->Intersect(ray, isect))
		{
			intersected = true;
			isect.primitive = primitive;
		}
	}

	if (intersected)
	{
		// Fill in the information in isect
		// Compute conversion to/from shading coordinates
		isect.worldToShading = Math::Transpose(Mat3d(isect.ss, isect.st, isect.sn));
		isect.shadingToWorld = Math::Inverse(isect.worldToShading);
	}

	return intersected;
}

HINATA_NAMESPACE_END