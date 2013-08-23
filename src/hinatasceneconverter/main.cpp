#include "colladaloader.h"
#include <hinatacore/scenedata.h>
#include <iostream>
#include <fstream>
#include <boost/format.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

using namespace hinata;

int main()
{
	ColladaLoader loader;
	ColladaLoader::LoadParam param;

	param.path = "resources/201305_tableset_1.dae";
	param.aspect = 1.0;
	param.bgColor = Vec3d(1.0);
	param.envMapPath = "resources/rnl_probe_panorama.hdr";
	param.envMapOffset = 0.3;
	param.envMapScale = 15.0;

	loader.Load(param);
	std::cout << "Load completed" << std::endl;

	// --------------------------------------------------------------------------------

	// Serialize scene
	std::ofstream ofs("scene.hinata", std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

	if (!ofs)
	{
		std::cerr << "std::ofstream" << std::endl;
		return 1;
	}

	try
	{
		boost::archive::binary_oarchive oa(ofs);
		oa & boost::serialization::make_nvp("HinataScene", loader.GetSceneData());
	}
	catch (boost::archive::archive_exception& e)
	{
		std::cerr << boost::str(boost::format("boost::archive::xml_oarchive : %s") % e.what()) << std::endl;;
		return 1;
	}

	return 0;
}