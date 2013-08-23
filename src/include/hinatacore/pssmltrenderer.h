#ifndef __HINATA_CORE_PSSMLT_RENDERER_H__
#define __HINATA_CORE_PSSMLT_RENDERER_H__

#include "renderer.h"
#include <string>

HINATA_NAMESPACE_BEGIN

enum class PSSMLTEstimatorMode
{
	Normal,
	MeanValueSubstitution,
	MeanValueSubstitution_LargeStepMIS
};

class PSSMLTRendererConfig : public RendererConfig
{
public:

	PSSMLTRendererConfig();

public:

	void DefineOptions(boost::program_options::options_description& opt);
	void ParseOptions(boost::program_options::variables_map& vm);

public:

	// Options
	int numMutations;
	int numSeedSamples;
	int rrDepth;
	double largeStepProb;
	PSSMLTEstimatorMode estimatorMode;
	double kernelSizeS1;
	double kernelSizeS2;

};


// ------------------------------------------------------------------------------------------

class Sampler;
class RestorableSampler;
class LazyPSSSampler;
class Ray;

class PSSMLTRenderer : public Renderer
{
public:

	// In order to reduce memory usage for holding path seeds,
	// we save the generated sample index and restore the path afterwards.
	struct PathSeed
	{
		PathSeed() {}
		PathSeed(int index, double I)
			: index(index)
			, I(I)
		{}

		int index;
		double I;
	};

	struct PathSampleRecord
	{
		Vec2i pixelPos;
		Vec3d L;
		double I;
	};

	struct PSSMLT_Thread_InitParam : public Thread_InitParam  
	{
		PathSeed seed;
		std::shared_ptr<RestorableSampler> rSampler;
	};

	struct PSSMLT_Thread_SharedData : public Thread_SharedData
	{
		PathSampleRecord record[2];
		int current;
		std::shared_ptr<LazyPSSSampler> sampler;
	};

public:

	PSSMLTRenderer(std::shared_ptr<PSSMLTRendererConfig>& config);

private:

	void Preprocess();
	void SampleAndEvaluatePath(const std::shared_ptr<Sampler>& sampler, PathSampleRecord& record);
	void RenderPassFinished();
	void SaveImageFinished();
	double ImageSaveWeight();
	std::shared_ptr<Thread_InitParam> Create_Thread_InitParam(int id);
	std::shared_ptr<Thread_SharedData> Create_Thread_SharedData();
	void InitializeThread(std::shared_ptr<Thread_InitParam>& p, std::shared_ptr<Thread_SharedData>& s);
	void ProcessThread_Render(std::shared_ptr<Thread_SharedData>& s);
	void AccumulateColor(std::shared_ptr<PSSMLT_Thread_SharedData>& shared, PathSampleRecord& record, double weight);

private:

	std::shared_ptr<PSSMLTRendererConfig> config;
	long long totalMutations;
	std::vector<PathSeed> seeds;
	double b;
	std::shared_ptr<RestorableSampler> rSampler;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_PSSMLT_RENDERER_H__