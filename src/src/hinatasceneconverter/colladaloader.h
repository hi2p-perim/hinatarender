#ifndef __HINATA_SCENE_CONVERTER_COLLADA_LOADER_H__
#define __HINATA_SCENE_CONVERTER_COLLADA_LOADER_H__

#include <hinatacore/common.h>
#include <hinatacore/math.h>
#include <string>
#include <memory>
#include <boost/shared_ptr.hpp>

struct aiScene;
struct aiNode;

HINATA_NAMESPACE_BEGIN

struct SceneData;

class ColladaLoader
{
public:

	struct LoadParam
	{
		std::string path;			// Path to the COLLADA dae file
		double aspect;				// Aspect ratio of the screen
		std::string envMapPath;		// Path to the environment map, if any
		double envMapOffset;		// Offset of the environment map
		double envMapScale;			// Radiance scale of the environment map
		Vec3d bgColor;				// Background color
	};

public:

	ColladaLoader();

private:

	ColladaLoader(const ColladaLoader&);
	ColladaLoader(ColladaLoader&&);
	void operator=(const ColladaLoader&);
	void operator=(ColladaLoader&&);

public:

	void Load(const LoadParam& param);
	boost::shared_ptr<SceneData> GetSceneData() { return sceneData; }

private:

	void Load(const aiScene* scene, const aiNode* node, const Mat4d& worldTransform);

private:

	std::string firstCameraName;
	LoadParam param;
	boost::shared_ptr<SceneData> sceneData;

};

HINATA_NAMESPACE_END

#endif // __HINATA_SCENE_CONVERTER_DAE_LOADER_H__
