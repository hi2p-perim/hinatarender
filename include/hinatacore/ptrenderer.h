#ifndef __HINATA_CORE_PT_RENDERER_H__
#define __HINATA_CORE_PT_RENDERER_H__

#include "renderer.h"

HINATA_NAMESPACE_BEGIN

class PTRendererConfig : public RendererConfig
{
public:

	PTRendererConfig();
	void DefineOptions(boost::program_options::options_description& opt);
	void ParseOptions(boost::program_options::variables_map& vm);

public:

	// Options
	int samplePerTask;
	int rrDepth;

};

// --------------------------------------------------------------------------------

class Ray;

class PTRenderer : public Renderer
{
public:

	PTRenderer(const std::shared_ptr<PTRendererConfig>& config);

private:

	void Preprocess();
	void RenderPassFinished();
	double ImageSaveWeight();
	void InitializeThread(std::shared_ptr<Thread_InitParam>& param, std::shared_ptr<Thread_SharedData>& shared);
	void ProcessThread_Render(std::shared_ptr<Thread_SharedData>& shared);

private:

	Vec3d Li(Ray& initialRay, std::shared_ptr<Thread_SharedData>& shared);

public:

	std::shared_ptr<PTRendererConfig> config;
	long long processedSamples;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_PT_RENDERER_H__