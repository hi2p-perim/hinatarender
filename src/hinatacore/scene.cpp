#include "pch.h"
#include <hinatacore/scene.h>

HINATA_NAMESPACE_BEGIN

void Scene::SampleLight( double& u, std::shared_ptr<AreaLight>& light, double& pdf )
{
	int n = (int)lights.size();
	int index = std::min((int)std::floor(u * n), n - 1);

	// u' = (u - delta * i) / delta
	u = u * n - (double)index;
	light = lights[index];
	pdf = 1.0 / n;
}

HINATA_NAMESPACE_END