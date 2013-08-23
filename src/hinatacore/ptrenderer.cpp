#include "pch.h"
#include <hinatacore/ptrenderer.h>
#include <hinatacore/ray.h>
#include <hinatacore/random.h>
#include <hinatacore/scene.h>
#include <hinatacore/perspectivecamera.h>
#include <hinatacore/arealight.h>
#include <hinatacore/environmentlight.h>
#include <hinatacore/intersection.h>
#include <hinatacore/primitive.h>
#include <hinatacore/bsdf.h>
#include <hinatacore/renderutils.h>

HINATA_NAMESPACE_BEGIN

PTRendererConfig::PTRendererConfig()
{
	appName = "pt";
	samplePerTask = 1000;
	rrDepth = 3;
}

void PTRendererConfig::DefineOptions( boost::program_options::options_description& opt )
{
	namespace po = boost::program_options;

	opt.add_options()
		("sample-per-task", po::value<int>(), "Sample per task")
		("rr-depth", po::value<int>(), "Depth to enable RR for path termination");
}

void PTRendererConfig::ParseOptions( boost::program_options::variables_map& vm )
{
	if (vm.count("sample-per-task"))
		samplePerTask = vm["sample-per-task"].as<int>();
	if (vm.count("rr-depth"))
		rrDepth = vm["rr-depth"].as<int>();
}

// --------------------------------------------------------------------------------

PTRenderer::PTRenderer( const std::shared_ptr<PTRendererConfig>& config )
	: Renderer(config)
	, config(config)
{

}

void PTRenderer::Preprocess()
{
	processedSamples = 0;
}

void PTRenderer::RenderPassFinished()
{
	processedSamples += config->samplePerTask * config->numRenderTasks;
}

double PTRenderer::ImageSaveWeight()
{
	return (double)(config->width * config->height) / processedSamples;
}

void PTRenderer::InitializeThread( std::shared_ptr<Thread_InitParam>& param, std::shared_ptr<Thread_SharedData>& shared )
{

}

void PTRenderer::ProcessThread_Render( std::shared_ptr<Thread_SharedData>& shared )
{
	Ray initialRay;

	for (int i = 0; i < config->samplePerTask; i++)
	{
		// Raster position
		Vec2d rasterPos(shared->rng->Next(), shared->rng->Next());

		int x = (int)(rasterPos.x * config->width);
		int y = (int)(rasterPos.y * config->height);

		// Generate ray
		double _;
		scene->Camera()->SampleAndEvaluate(rasterPos, initialRay, _);

		// Evaluate radiance and accumulate
		shared->color[y * config->width + x] += Li(initialRay, shared);
	}
}

// --------------------------------------------------------------------------------

Vec3d PTRenderer::Li( Ray& initialRay, std::shared_ptr<Thread_SharedData>& shared )
{
	Ray ray = initialRay;
	Intersection isect;

	Vec3d L;
	Vec3d throughput(1.0);
	int depth = 0;

	while (true)
	{
		// Check intersection
		if (!scene->Intersect(ray, isect))
		{
			// There is no intersection, evaluate environment light if exists.
			auto& envLight = scene->GetEnvironmentLight();

			if (envLight != nullptr)
			{
				L += throughput * envLight->Evaluate(-ray.d);
			}

			break;
		}

		auto light = isect.primitive->Light();

		if (light != nullptr)
		{
			// Emission
			L += throughput * light->Evaluate(-ray.d, isect.gn);
		}

		// ----------------------------------------------------------------------

		// Sample BSDF
		std::shared_ptr<BSDF> bsdf = isect.primitive->Bsdf();

		BSDFSample sample;
		sample.u = Vec2d(shared->rng->Next(), shared->rng->Next());
		sample.uComponent = shared->rng->Next();

		BSDFRecord record;
		record.type = BSDFType::All;
		record.adjoint = false;
		record.wi = Math::Normalize(isect.worldToShading * -ray.d);

		double pdf;
		auto weight = bsdf->SampleAndEvaluate(record, sample, pdf, isect);

		if (pdf == 0.0 || weight == Vec3d())
		{
			break;
		}

		// Convert to world coordinates
		record.wo = Math::Normalize(isect.shadingToWorld * record.wo);

		// Update throughput
		throughput *= weight;

		// Setup next ray
		ray.d = record.wo;
		ray.o = isect.p;
		ray.minT = isect.rayEpsilon;
		ray.maxT = Inf;

		// ----------------------------------------------------------------------

		if (++depth >= config->rrDepth)
		{
			// Russian roulette for path termination
			double p = std::min(0.5, RenderUtils::Luminance(throughput));

			if (shared->rng->Next() > p)
			{
				break;
			}

			throughput /= Vec3d(p);
		}
	}

	return L;
}

HINATA_NAMESPACE_END