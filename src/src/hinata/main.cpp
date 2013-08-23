#include <hinatacore/pssmltrenderer.h>
//#include <hinatacore/ptrenderer.h>
#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
	try
	{
		auto config = std::make_shared<hinata::PSSMLTRendererConfig>();
		//auto config = std::make_shared<hinata::PTRendererConfig>();
		if (config->ProcessArgs(argc, argv))
		{
			hinata::PSSMLTRenderer(config).Render();
			//hinata::PTRenderer(config).Render();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cin.get();
	}

	return 0;
}
