#ifndef __HINATA_CORE_SCENE_DATA_H__
#define __HINATA_CORE_SCENE_DATA_H__

#include "common.h"
#include "math.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

// Serialization for vectors and matrices
namespace boost
{
namespace serialization
{

	template <class Archive, typename T>
	void serialize(Archive& ar, hinata::Vec2<T>& v, unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(v.x);
		ar & BOOST_SERIALIZATION_NVP(v.y);
	}

	template <class Archive, typename T>
	void serialize(Archive& ar, hinata::Vec3<T>& v, unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(v.x);
		ar & BOOST_SERIALIZATION_NVP(v.y);
		ar & BOOST_SERIALIZATION_NVP(v.z);
	}

	template <class Archive, typename T>
	void serialize(Archive& ar, hinata::Vec4<T>& v, unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(v.x);
		ar & BOOST_SERIALIZATION_NVP(v.y);
		ar & BOOST_SERIALIZATION_NVP(v.z);
		ar & BOOST_SERIALIZATION_NVP(v.w);
	}

	template <class Archive, typename T>
	void serialize(Archive& ar, hinata::Mat3<T>& m, unsigned int version)
	{
		ar & boost::serialization::make_nvp("v0", m[0]);
		ar & boost::serialization::make_nvp("v1", m[1]);
		ar & boost::serialization::make_nvp("v2", m[2]);
	}

	template <class Archive, typename T>
	void serialize(Archive& ar, hinata::Mat4<T>& m, unsigned int version)
	{
		ar & boost::serialization::make_nvp("v0", m[0]);
		ar & boost::serialization::make_nvp("v1", m[1]);
		ar & boost::serialization::make_nvp("v2", m[2]);
		ar & boost::serialization::make_nvp("v3", m[3]);
	}

}
}

HINATA_NAMESPACE_BEGIN

struct SceneDataElement_TriangleMesh
{

	std::vector<Vec3d> positions;
	std::vector<Vec3d> normals;
	std::vector<Vec2d> texcoords;
	std::vector<Vec3i> faces;	// Indices for each face
	bool oneSided;
	int materialIndex;
	
private:

	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive& ar, unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(positions);
		ar & BOOST_SERIALIZATION_NVP(normals);
		ar & BOOST_SERIALIZATION_NVP(texcoords);
		ar & BOOST_SERIALIZATION_NVP(faces);
		ar & BOOST_SERIALIZATION_NVP(oneSided);
		ar & BOOST_SERIALIZATION_NVP(materialIndex);
	}

};

enum class SceneDataElement_MaterialType
{
	DiffuseBSDF_Texture,
	DiffuseBSDF_Color,
	PerfectMirrorBSDF,
	DielecticBSDF,
	GlossyConductorBSDF,
	Emissive
};

struct SceneDataElement_Material
{

	SceneDataElement_MaterialType type;
	std::string texturePath;
	Vec3d color;
	Vec3d emissiveColor;
	double ior;
	double shininess;

private:

	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive& ar, unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(type);
		ar & BOOST_SERIALIZATION_NVP(texturePath);
		ar & BOOST_SERIALIZATION_NVP(color);
		ar & BOOST_SERIALIZATION_NVP(emissiveColor);
		ar & BOOST_SERIALIZATION_NVP(ior);
		ar & BOOST_SERIALIZATION_NVP(shininess);
	}

};

struct SceneDataElement_EnvMap
{
	
	std::string path;
	Vec3d bgColor;
	double offset;
	double scale;

private:

	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive& ar, unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(path);
		ar & BOOST_SERIALIZATION_NVP(bgColor);
		ar & BOOST_SERIALIZATION_NVP(offset);
		ar & BOOST_SERIALIZATION_NVP(scale);
	}

};

struct SceneDataElement_Camera
{

	Mat4d viewMatrix;
	Mat4d projectionMatrix;

private:

	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive& ar, unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(viewMatrix);
		ar & BOOST_SERIALIZATION_NVP(projectionMatrix);
	}

};

struct SceneDataElement_Primitive
{

	Mat4d transform;
	std::vector<int> meshIndices;

private:

	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive& ar, unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(transform);
		ar & BOOST_SERIALIZATION_NVP(meshIndices);
	}

};

struct SceneData
{

	SceneDataElement_EnvMap envMap;
	SceneDataElement_Camera camera;
	std::vector<boost::shared_ptr<SceneDataElement_TriangleMesh>> meshes;
	std::vector<boost::shared_ptr<SceneDataElement_Material>> materials;
	std::vector<boost::shared_ptr<SceneDataElement_Primitive>> primitives;

private:

	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive& ar, unsigned int version)
	{
		ar & BOOST_SERIALIZATION_NVP(envMap);
		ar & BOOST_SERIALIZATION_NVP(camera);
		ar & BOOST_SERIALIZATION_NVP(meshes);
		ar & BOOST_SERIALIZATION_NVP(materials);
		ar & BOOST_SERIALIZATION_NVP(primitives);
	}

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_SCENE_DATA_H__