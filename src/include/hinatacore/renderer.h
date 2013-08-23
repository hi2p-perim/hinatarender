#ifndef __HINATA_CORE_RENDERER_H__
#define __HINATA_CORE_RENDERER_H__

#include "common.h"
#include "math.h"
#include "syncqueue.h"
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <boost/program_options.hpp>

HINATA_NAMESPACE_BEGIN

class RendererConfig
{
public:

	RendererConfig();
	bool ProcessArgs(int argc, char** argv);
	virtual void DefineOptions(boost::program_options::options_description& opt) = 0;
	virtual void ParseOptions(boost::program_options::variables_map& vm) = 0;

public:

	// Common options
	std::string appName;
	bool quiet;
	int width;
	int height;
	std::string outputDir;
	std::string outputFilePrefix;
	bool disableOutputFileSuffix;
	bool fixedScene;
	std::string scenePath;
	int numThreads;
	int numRenderTasks;
	double executionTime;
	double imageSaveIntervalTime;

	// Scene configuration (should not be here)
	std::string envMapPath;
	double envMapOffset;
	double envMapScale;
	Vec3d bgColor;

};

// --------------------------------------------------------------------------------

class Random;
class Image;
class Scene;

class Renderer
{
public:

	enum class Command
	{
		Render,
		UpdateImage
	};

	struct Task
	{
		Command command;
		bool needSync;
	};

	struct Thread_InitParam
	{
		virtual ~Thread_InitParam() {}
		int id;
	};

	struct Thread_SharedData
	{
		virtual ~Thread_SharedData() {}
		std::vector<Vec3d> color;
		std::shared_ptr<Random> rng;
	};

public:

	Renderer(const std::shared_ptr<RendererConfig>& config);
	virtual ~Renderer();

private:

	Renderer(const Renderer&);
	Renderer(Renderer&&);
	void operator=(const Renderer&);
	void operator=(Renderer&&);

public:

	void Render();

private:

	virtual void Preprocess() = 0;
	virtual void RenderPassFinished() = 0;
	virtual void SaveImageFinished() {}
	virtual double ImageSaveWeight() = 0;
	virtual void InitializeThread(std::shared_ptr<Thread_InitParam>& param, std::shared_ptr<Thread_SharedData>& shared) {}
	virtual void ProcessThread_Render(std::shared_ptr<Thread_SharedData>& shared) = 0;
	virtual std::shared_ptr<Thread_InitParam> Create_Thread_InitParam(int id) { return std::make_shared<Thread_InitParam>(); }
	virtual std::shared_ptr<Thread_SharedData> Create_Thread_SharedData() { return std::make_shared<Thread_SharedData>(); }

private:

	void ProcessThread(std::shared_ptr<Thread_InitParam> param);

protected:

	std::shared_ptr<RendererConfig> commonConfig;

	std::unique_ptr<Image> image;
	std::unique_ptr<Scene> scene;

	SyncQueue<Task> queue;
	std::vector<std::shared_ptr<Thread_SharedData>> threadSharedData;

	int finishedTasks;
	std::mutex taskFinishedMutex;
	std::condition_variable taskFinished;

	int waitingThreads;
	std::mutex taskSyncMutex;
	std::condition_variable taskSync;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_RENDERER_H__