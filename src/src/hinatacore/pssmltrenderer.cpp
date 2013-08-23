#include "pch.h"
#include <hinatacore/pssmltrenderer.h>
#include <hinatacore/pssmltsampler.h>
#include <hinatacore/random.h>
#include <hinatacore/ray.h>
#include <hinatacore/intersection.h>
#include <hinatacore/primitive.h>
#include <hinatacore/scene.h>
#include <hinatacore/renderutils.h>
#include <hinatacore/perspectivecamera.h>
#include <hinatacore/arealight.h>
#include <hinatacore/environmentlight.h>
#include <hinatacore/bsdf.h>

HINATA_NAMESPACE_BEGIN

PSSMLTRendererConfig::PSSMLTRendererConfig()
{
	appName = "pssmlt";
	numMutations = 10000;
	numSeedSamples = 1000000;
	rrDepth = 0;
	largeStepProb = 0.3;
	//estimatorMode = PSSMLTEstimatorMode::Normal;
	estimatorMode = PSSMLTEstimatorMode::MeanValueSubstitution_LargeStepMIS;
	kernelSizeS1 = 4.0 / 1024.0;
	kernelSizeS2 = 4.0 / 64.0;
}

void PSSMLTRendererConfig::DefineOptions( boost::program_options::options_description& opt )
{
	namespace po = boost::program_options;

	opt.add_options()
		("num-mutations", po::value<int>(), "Number of mutations per task")
		("num-seed-samples", po::value<int>(), "Number of mutations per thread")
		("rr-depth", po::value<int>(), "Depth to enable RR for path termination")
		("large-step-prob", po::value<double>(), "Large step mutation probability")
		("estimator-mode", po::value<std::string>(), "Estimator mode (normal, mvs, mvs-mis")
		("kernel-size-s1", po::value<double>(), "Minimum kernel size")
		("kernel-size-s2", po::value<double>(), "Maximum kernel size");
}

void PSSMLTRendererConfig::ParseOptions( boost::program_options::variables_map& vm )
{
	if (vm.count("num-mutations"))
		numMutations = vm["num-mutations"].as<int>();
	if (vm.count("num-seed-samples"))
		numSeedSamples = vm["num-seed-samples"].as<int>();
	if (vm.count("rr-depth"))
		rrDepth = vm["rr-depth"].as<int>();
	if (vm.count("large-step-prob"))
		largeStepProb = vm["large-step-prob"].as<double>();

	if (vm.count("estimator-mode"))
	{
		std::string str = vm["estimator-mode"].as<std::string>();
		if (str == "normal")
			estimatorMode = PSSMLTEstimatorMode::Normal;
		else if (str == "mvs")
			estimatorMode = PSSMLTEstimatorMode::MeanValueSubstitution;
		else if (str == "mvs-mis")
			estimatorMode = PSSMLTEstimatorMode::MeanValueSubstitution_LargeStepMIS;
		else
		{
			std::cerr << "Invalid mode, setting to normal" << std::endl;
			estimatorMode = PSSMLTEstimatorMode::Normal;
		}
	}

	if (vm.count("kernel-size-s1"))
		kernelSizeS1 = vm["kernel-size-s1"].as<double>();
	if (vm.count("kernel-size-s2"))
		kernelSizeS2 = vm["kernel-size-s2"].as<double>();
}

// ------------------------------------------------------------------------------------------


PSSMLTRenderer::PSSMLTRenderer( std::shared_ptr<PSSMLTRendererConfig>& config )
	: Renderer(config)
	, config(config)
{

}

void PSSMLTRenderer::Preprocess()
{
	totalMutations = 0;

	// Restorable sampler
	rSampler = std::make_shared<RestorableSampler>();

	// Generate seeds
	// As well as seeds we compute the variable b,
	// the integral of I over the sample space, using path tracing.

	std::vector<PathSeed> candidates;
	PathSampleRecord record;
	double sumI = 0;

	std::cerr << "Generating seeds ..." << std::endl;

	for (int i = 0; i < config->numSeedSamples; i++)
	{
		// Current index before sampling a path
		int index = rSampler->Index();

		// Sample the path and evaluate radiance
		SampleAndEvaluatePath(rSampler, record);

		sumI += record.I;

		if (record.L != Vec3d())
		{
			candidates.push_back(PathSeed(index, record.I));
		}

		if (i % 100 == 0 || i == config->numSeedSamples - 1)
		{
			std::cerr <<
				(boost::format("\rProgress : %.2lf %%")
				% ((double)(i + 1) / config->numSeedSamples * 100.0)).str();
		}
	}

	b = sumI / config->numSeedSamples;

	std::cerr << std::endl << std::endl;

	// --------------------------------------------------------------------------------

	// Sample seeds according to I

	// Create CDF
	std::vector<double> cdf(1, 0.0);
	for (auto& candidate : candidates)
	{
		cdf.push_back(cdf.back() + candidate.I);
	}

	// Normalize
	double sum = cdf.back();
	for (double& v : cdf)
	{
		v /= sum;
	}

	// Sample seeds
	// #seeds = #threads
	for (int i = 0; i < config->numThreads; i++)
	{
		double u = rSampler->Next();
		int idx =
			Math::Clamp(
			std::upper_bound(cdf.begin(), cdf.end(), u) - cdf.begin() - 1,
			0, (int)cdf.size() - 1);

		seeds.push_back(candidates[idx]);
	}
}

void PSSMLTRenderer::SampleAndEvaluatePath( const std::shared_ptr<Sampler>& sampler, PathSampleRecord& record )
{
	Ray ray;
	Intersection isect;
	Vec3d throughput(1.0);
	int depth = 0;
	Vec3d L;

	// Raster position
	Vec2d rasterPos(sampler->Next(), sampler->Next());
	
	record.pixelPos.x = (int)(rasterPos.x * config->width);
	record.pixelPos.y = (int)(rasterPos.y * config->height);

	// Generate ray
	double _;
	scene->Camera()->SampleAndEvaluate(rasterPos, ray, _);

	// ----------------------------------------------------------------------

	// Initial intersection
	if (!scene->Intersect(ray, isect))
	{
		record.L = L;
		record.I = RenderUtils::Luminance(L);
		return;
	}

	if (isect.primitive->Light() != nullptr)
	{
		auto& light = isect.primitive->Light();
		L += light->Evaluate(-ray.d, isect.gn);
	}

	// ----------------------------------------------------------------------

	while (true)
	{
		auto& bsdf = isect.primitive->Bsdf();

		// Explicit (direct) light sampling
		// We do not handle the light path with length 1 (EL path)

		if (isect.primitive != nullptr)
		{
			auto positionSample = Vec2d(sampler->Next(), sampler->Next());

			// Sample a light
			std::shared_ptr<AreaLight> light;
			double lightSelectionPdf;
			scene->SampleLight(positionSample.x, light, lightSelectionPdf);

			// Sample a position on the light
			AreaLight::SampleRecord lightSampleRec;
			lightSampleRec.positionSample = positionSample;
			light->SamplePosition(lightSampleRec);

			// Check visibility
			Ray shadowRay;
			auto d = lightSampleRec.p - isect.p;
			shadowRay.d = Math::Normalize(d);
			shadowRay.o = isect.p;
			shadowRay.minT = isect.rayEpsilon;
			shadowRay.maxT = Math::Length(d) * (1.0 - Eps);

			Intersection shadowIsect;

			if (!scene->Intersect(shadowRay, shadowIsect))
			{
				// Evaluate Le (with cosine term)
				auto Le = light->EvaluateCos(-shadowRay.d, lightSampleRec.n) / lightSelectionPdf;

				// Convert to Le / p_\sigma
				auto dist2 = Math::Length2(d);
				Le /= Vec3d(dist2);

				// Prepare for BSDF evaluation
				BSDFRecord bsdfRec;
				bsdfRec.type = BSDFType::All;
				bsdfRec.adjoint = false;
				bsdfRec.wi = isect.worldToShading * -ray.d;
				bsdfRec.wo = isect.worldToShading * shadowRay.d;

				// Evaluate BSDF (with cosine term)
				auto f = bsdf->Evaluate(bsdfRec, isect);

				if (f != Vec3d())
				{
					// Calculate PDF for light and BSDF (in solid angle measure)
					double dDotN = Math::Dot(-shadowRay.d, lightSampleRec.n);
					double lightPdf =
						dDotN <= 0
							? 0.0
							: lightSelectionPdf *		// Selection
								lightSampleRec.pdf *	// p_A(x_n)
								dist2 / dDotN;			// Convert to p_\sigma(x_{n-1}\to x_n)

					if (lightPdf > 0)
					{
						// It should be positive
						double bsdfPdf = bsdf->Pdf(bsdfRec);

						// MIS weight (for direct light sampling)
						double w = lightPdf / (lightPdf + bsdfPdf);

						// Record color
						L += throughput * f * Le * w;
					}
				}
			}
		}

		// ----------------------------------------------------------------------

		// BSDF sampling

		BSDFSample bsdfSample;
		bsdfSample.u = Vec2d(sampler->Next(), sampler->Next());
		bsdfSample.uComponent = sampler->Next();

		BSDFRecord bsdfRec;
		bsdfRec.type = BSDFType::All;
		bsdfRec.adjoint = false;
		bsdfRec.wi = isect.worldToShading * -ray.d;

		double bsdfPdf;
		auto f = bsdf->SampleAndEvaluate(bsdfRec, bsdfSample, bsdfPdf, isect);

		if (bsdfPdf == 0.0 || f == Vec3d())
		{
			record.L = L;
			record.I = RenderUtils::Luminance(L);
			return;
		}

		// Convert to world coordinates
		bsdfRec.wo = isect.shadingToWorld * bsdfRec.wo;

		// Update throughput
		throughput *= f;

		// Setup next ray
		ray.d = bsdfRec.wo;
		ray.o = isect.p;
		ray.minT = isect.rayEpsilon;
		ray.maxT = Inf;

		// ----------------------------------------------------------------------

		// Check intersection
		if (!scene->Intersect(ray, isect))
		{
			record.L = L;
			record.I = RenderUtils::Luminance(L);
			return;
		}

		auto light = isect.primitive->Light();

		if (light != nullptr)
		{
			// Intersected with L

			// Evaluate Le
			auto Le = light->Evaluate(-ray.d, isect.gn);

			if ((bsdfRec.sampledType & BSDFType::Delta) != 0)
			{
				// Disable MIS if last intersection is specular interaction
				L += throughput * Le;
			}
			else
			{
				// Calculate PDF for light (in solid angle measure)
				double dist2 = Math::Length2(ray.o - isect.p);
				double dDotN = Math::Dot(-ray.d, isect.gn);
				double lightPdf =
					dDotN <= 0
						? 0.0
						: scene->LightSelectionPdf() *	// Selection
							light->PdfPosition() *		// p_A(x_n)
							dist2 / dDotN;				// Convert to p_\sigma(x_{n-1}\to x_n)

				// MIS weight (for BSDF sampling)
				double w = bsdfPdf / (lightPdf + bsdfPdf);

				// Record color
				L += throughput * Le * w;
			}
		}

		// ----------------------------------------------------------------------

		if (++depth >= config->rrDepth)
		{
			// Russian roulette for path termination
			double p = Math::Min(0.5, RenderUtils::Luminance(throughput));

			if (sampler->Next() > p)
			{
				break;
			}

			throughput /= Vec3d(p);
		}
	}

	record.L = L;
	record.I = RenderUtils::Luminance(L);
}

void PSSMLTRenderer::RenderPassFinished()
{
	totalMutations += config->numMutations * config->numRenderTasks;
}

void PSSMLTRenderer::SaveImageFinished()
{

}

double PSSMLTRenderer::ImageSaveWeight()
{
	return (double)(config->width * config->height) / totalMutations;
}

std::shared_ptr<PSSMLTRenderer::Thread_InitParam> PSSMLTRenderer::Create_Thread_InitParam( int id )
{
	auto param = std::make_shared<PSSMLT_Thread_InitParam>();
	param->seed = seeds[id];
	param->rSampler = std::make_shared<RestorableSampler>(*rSampler);
	return param;
}

std::shared_ptr<PSSMLTRenderer::Thread_SharedData> PSSMLTRenderer::Create_Thread_SharedData()
{
	return std::make_shared<PSSMLT_Thread_SharedData>();
}

void PSSMLTRenderer::InitializeThread( std::shared_ptr<Thread_InitParam>& p, std::shared_ptr<Thread_SharedData>& s )
{
	auto param = std::dynamic_pointer_cast<PSSMLT_Thread_InitParam>(p);
	auto shared = std::dynamic_pointer_cast<PSSMLT_Thread_SharedData>(s);

	// Initialize LazyPSSSampler

	shared->current = 0;

	// Replacing the random number generator of LazyPSSSampler with 
	// that of RestorableSampler, LazyPSSSampler reproduces the seed samples and
	// the samples are saved as an initial state of LazyPSSSampler.

	shared->sampler = std::make_shared<LazyPSSSampler>(config->kernelSizeS1, config->kernelSizeS2);

	param->rSampler->SetIndex(param->seed.index);
	shared->sampler->SetRng(param->rSampler->Rng());

	SampleAndEvaluatePath(shared->sampler, shared->record[shared->current]);
	assert(param->seed.I == shared->record[shared->current].I);

	shared->sampler->SetRng(shared->rng);
	shared->sampler->Accept();
}

void PSSMLTRenderer::ProcessThread_Render( std::shared_ptr<Thread_SharedData>& s )
{
	auto shared = std::dynamic_pointer_cast<PSSMLT_Thread_SharedData>(s);

	for (int i = 0; i < config->numMutations; i++)
	{
		PathSampleRecord& current = shared->record[shared->current];
		PathSampleRecord& proposed = shared->record[1-shared->current];

		// --------------------------------------------------------------------------------

		bool largeStep = shared->rng->Next() < config->largeStepProb;

		shared->sampler->SetLargeStep(largeStep);
		SampleAndEvaluatePath(shared->sampler, proposed);

		// --------------------------------------------------------------------------------

		// Acceptance ratio
		double a = current.I > 0 ? std::min(1.0, proposed.I / current.I) : 1.0;

		if (config->estimatorMode == PSSMLTEstimatorMode::MeanValueSubstitution)
		{
			AccumulateColor(shared, current, (1 - a) / current.I * b);
			AccumulateColor(shared, proposed, a / proposed.I * b);
		}
		else if (config->estimatorMode == PSSMLTEstimatorMode::MeanValueSubstitution_LargeStepMIS)
		{
			AccumulateColor(shared, current, (1 - a) / (current.I / b + config->largeStepProb));
			AccumulateColor(shared, proposed, (a + (largeStep ? 1 : 0)) / (proposed.I / b + config->largeStepProb));
		}

		// --------------------------------------------------------------------------------

		if (shared->rng->Next() < a)
		{
			// Accepted
			shared->sampler->Accept();
			shared->current = 1 - shared->current;
		}
		else
		{
			// Rejected
			shared->sampler->Reject();
		}

		// --------------------------------------------------------------------------------

		if (config->estimatorMode == PSSMLTEstimatorMode::Normal)
		{
			auto& c = shared->record[shared->current];
			AccumulateColor(shared, c, b / c.I);
		}
	}
}

void PSSMLTRenderer::AccumulateColor( std::shared_ptr<PSSMLT_Thread_SharedData>& shared, PathSampleRecord& record, double weight )
{
	Vec2i& p = record.pixelPos;

	if (record.I > 0)
	{
		shared->color[p.y * config->width + p.x] += record.L * weight;
	}
}

HINATA_NAMESPACE_END