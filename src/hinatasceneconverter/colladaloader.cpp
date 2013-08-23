#include "colladaloader.h"

#include <hinatacore/scenedata.h>

#include <regex>

#include <boost/make_shared.hpp>
#include <boost/unordered_map.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

HINATA_NAMESPACE_BEGIN

ColladaLoader::ColladaLoader()
{

}

void ColladaLoader::Load( const LoadParam& param )
{
	// Load params
	this->param = param;

	// Create scene data
	sceneData = boost::make_shared<SceneData>();

	// Create Assimp importer and read Collada file
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(param.path,
		aiProcess_GenSmoothNormals |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		throw std::invalid_argument("Failed to load scene");
	}

	//std::regex containsOnesidedRegex("^.+\\.ONESIDED(\\..+)?$");
	std::regex containsOnesidedRegex("^.+ONESIDED(.+)?$");
	std::smatch match;

	// Meshes
	std::cout << "Loading meshes ..." << std::endl;

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[i];
		auto tm = boost::make_shared<SceneDataElement_TriangleMesh>();

		// Positions and normals
		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			auto& p = mesh->mVertices[j];
			auto& n = mesh->mNormals[j];
			tm->positions.push_back(Vec3d(p.x, p.y, p.z));
			tm->normals.push_back(Vec3d(n.x, n.y, n.z));
		}

		// Texture coordinates
		// Use only channel 0 if exists.
		if (mesh->HasTextureCoords(0))
		{
			for (unsigned int j = 0; j < mesh->mNumVertices; j++)
			{
				auto& uv = mesh->mTextureCoords[0][j];
				tm->texcoords.push_back(Vec2d(uv.x, uv.y));
			}	
		}

		// If the mesh name contains 'ONESIDED',
		// the mesh is one sided mesh, otherwise two sided mesh.
		std::string name(mesh->mName.C_Str());
		tm->oneSided = std::regex_match(name, match, containsOnesidedRegex);

		// Faces
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			auto& face = mesh->mFaces[j];
			assert(face.mNumIndices == 3);
			tm->faces.push_back(Vec3i(face.mIndices[0], face.mIndices[1], face.mIndices[2]));
		}

		// Material index
		tm->materialIndex = mesh->mMaterialIndex;

		sceneData->meshes.push_back(tm);
		std::cout << "  Loaded .. " << name << std::endl;
	}

	// Camera
	assert(scene->mNumCameras >= 1);
	firstCameraName = scene->mCameras[0]->mName.C_Str();

	// Materials
	//boost::unordered_map<std::string, std::shared_ptr<SceneDataElement_BitmapTexture>> texturePathMap;
	std::cout << "Loading materials ..." << std::endl;

	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		auto material = scene->mMaterials[i];

		auto materialData = boost::make_shared<SceneDataElement_Material>();

		// Emission term is replaced by AreaLight
		//glm::vec3 emission;

		aiString name;
		material->Get(AI_MATKEY_NAME, name);
		std::string materialName(name.C_Str()); 

		if (boost::starts_with(materialName, "DIFFUSE"))
		{
			// Has a texture?
			// Use only the initial texture (ID 0)
			aiString texturePath;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
			{
				// Create path (relative to current model path)
				namespace fs = boost::filesystem;
				auto imagePath = (fs::path(param.path).parent_path() / texturePath.C_Str()).string();

				materialData->type = SceneDataElement_MaterialType::DiffuseBSDF_Texture;
				materialData->texturePath = imagePath;
			}
			else
			{
				aiColor3D c;
				material->Get(AI_MATKEY_COLOR_DIFFUSE, c);
				
				materialData->type = SceneDataElement_MaterialType::DiffuseBSDF_Color;
				materialData->color = Vec3d(c.r, c.g, c.b);
			}
		}
		else if (boost::starts_with(materialName, "MIRROR"))
		{
			aiColor3D c;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, c);

			materialData->type = SceneDataElement_MaterialType::PerfectMirrorBSDF;
			materialData->color = Vec3d(c.r, c.g, c.b);
		}
		else if (boost::starts_with(materialName, "SPECULAR"))
		{
			aiColor3D c;
			float ior;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, c);
			material->Get(AI_MATKEY_REFRACTI, ior);

			materialData->type = SceneDataElement_MaterialType::DielecticBSDF;
			materialData->color = Vec3d(c.r, c.g, c.b);
			materialData->ior = static_cast<double>(ior);
		}
		else if (boost::starts_with(materialName, "GLOSSY"))
		{
			aiColor3D c;
			float shininess;
			material->Get(AI_MATKEY_COLOR_SPECULAR, c);
			material->Get(AI_MATKEY_SHININESS, shininess);

			materialData->type = SceneDataElement_MaterialType::GlossyConductorBSDF;
			materialData->color = Vec3d(c.r, c.g, c.b);
			materialData->shininess = static_cast<double>(shininess);
		}
		else if (boost::starts_with(materialName, "EMISSIVE"))
		{
			aiColor3D c1, c2;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, c1);
			material->Get(AI_MATKEY_COLOR_EMISSIVE, c2);

			materialData->type = SceneDataElement_MaterialType::Emissive;
			materialData->color = Vec3d(c1.r, c1.g, c1.b);
			materialData->emissiveColor = Vec3d(c2.r, c2.g, c2.b);
		}
		else
		{
			throw std::exception("Invalid material");
		}

		sceneData->materials.push_back(materialData);
		std::cout << "  Loaded .. " << materialName << std::endl;
	}

	// Nodes
	std::cout << "Loading primitives .." << std::endl;
	Load(scene, scene->mRootNode, Mat4d::Identity());

	sceneData->envMap.bgColor = param.bgColor;
	sceneData->envMap.path = param.envMapPath;
	sceneData->envMap.offset = param.envMapOffset;
	sceneData->envMap.scale = param.envMapScale;
}

void ColladaLoader::Load( const aiScene* scene, const aiNode* node, const Mat4d& worldTransform )
{
	auto primitiveData = boost::make_shared<SceneDataElement_Primitive>();

	auto localTransform = Mat4d(Math::ConvertToMat4(&(node->mTransformation.a1)));
	auto transform = worldTransform * Math::Transpose(localTransform);
	std::string nodeName = node->mName.C_Str();

	primitiveData->transform = transform;

	// Process camera
	if (nodeName == firstCameraName)
	{
		auto cm = scene->mCameras[0];
		auto lookat = Vec3d(transform * Vec4d(Vec3d(Math::ConvertToVec3(&(cm->mLookAt.x))), 0.0));
		auto position = Vec3d(transform * Vec4d(Vec3d(Math::ConvertToVec3(&(cm->mPosition.x))), 1.0));
		auto up = Vec3d(transform * Vec4d(Vec3d(Math::ConvertToVec3(&(cm->mUp.x))), 0.0));

		sceneData->camera.viewMatrix = Math::LookAt(position, position + lookat, up);
		sceneData->camera.projectionMatrix =
			Math::Perspective(
				Math::Degrees(static_cast<double>(cm->mHorizontalFOV)),
				param.aspect,
				(double)cm->mClipPlaneNear,
				(double)cm->mClipPlaneFar);
	}

	// Process meshes attached to the node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		primitiveData->meshIndices.push_back(node->mMeshes[i]);
	}

	sceneData->primitives.push_back(primitiveData);
	std::cout << "  Loaded .. " << nodeName << std::endl;

	// Process children recursively
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		Load(scene, node->mChildren[i], transform);
	}
}

HINATA_NAMESPACE_END