#include "pch.h"
#include <hinatacore/renderer.h>
#include <hinatacore/image.h>
#include <hinatacore/cornellboxscene.h>
#include <hinatacore/bvhscene.h>
#include <hinatacore/random.h>

HINATA_NAMESPACE_BEGIN

RendererConfig::RendererConfig()
{
	quiet = false;
	width = 1024;
	height = 1024;
	outputDir = ".";
	outputFilePrefix = "";
	disableOutputFileSuffix = false;
	fixedScene = true;
	scenePath = "scene.hinata";
#ifdef _DEBUG
	numThreads = 1;
	numRenderTasks = 2;
#else
	numThreads = std::thread::hardware_concurrency();
	numRenderTasks = 100;
#endif
	//executionTime = Inf;
	executionTime = 3540;
	imageSaveIntervalTime = 60.0;

	envMapPath = "";
	//envMapPath = "resources/grace_probe_panorama.hdr";
	envMapOffset = 0.0;
	envMapScale = 1.0;
	bgColor = Vec3d();
}

bool RendererConfig::ProcessArgs( int argc, char** argv )
{
	namespace po = boost::program_options;

	// Define options
	po::options_description opt(appName);
	opt.add_options()
		("help", "Display help message")
		("quiet", "Disable detailed messages")
		("width", po::value<int>(), "Width of the image")
		("height", po::value<int>(), "Height of the image")
		("output-dir", po::value<std::string>(), "Output directory")
		("output-file-prefix", po::value<std::string>(), "Prefix of output files (auto if empty)")
		("disable-output-file-suffix", "Disable suffix in output files")
		("fixed-scene", "Render fixed scene (Cornell Box)")
		("scene-path", po::value<std::string>(), "Path to the scene file")
		("num-threads", po::value<int>(), "Number of threads")
		("num-render-tasks", po::value<int>(), "Number of render tasks per pass")
		("execution-time", po::value<double>(), "Execution time (in seconds)")
		("image-save-interval-time", po::value<double>(), "Interval time to save an rendered image (in seconds)");

	opt.add_options()
		("env-map-path", po::value<std::string>(), "Path to the environment map")
		("env-map-offset", po::value<double>(), "Offset of the environment map")
		("env-map-scale", po::value<double>(), "Radiance scale of the environment map")
		("bg-color-r", po::value<double>(), "Red component of the background color")
		("bg-color-g", po::value<double>(), "Green component of the background color")
		("bg-color-b", po::value<double>(), "Blue component of the background color");

	DefineOptions(opt);

	// Parse options
	po::variables_map vm;
	po::store(boost::program_options::parse_command_line(argc, argv, opt), vm);
	po::notify(vm);

	if (vm.count("help"))
	{
		std::cerr << opt << std::endl;
		return false;
	}

	if (vm.count("width"))
		width = vm["width"].as<int>();
	if (vm.count("quiet"))
		quiet = true;
	if (vm.count("height"))
		height = vm["height"].as<int>();
	if (vm.count("output-dir"))
		outputDir = vm["output-dir"].as<std::string>();
	if (vm.count("output-file-prefix"))
		outputFilePrefix = vm["output-file-prefix"].as<std::string>();
	if (vm.count("disable-output-file-suffix"))
		disableOutputFileSuffix = true;
	if (vm.count("fixed-scene"))
		fixedScene = true;
	if (vm.count("scene-path"))
		scenePath = vm["scene-path"].as<std::string>();
	if (vm.count("num-threads"))
		numThreads = vm["num-threads"].as<int>();
	if (vm.count("num-render-tasks"))
		numRenderTasks = vm["num-render-tasks"].as<int>();
	if (vm.count("execution-time"))
		executionTime = vm["execution-time"].as<double>();
	if (vm.count("image-save-interval-time"))
		imageSaveIntervalTime = vm["image-save-interval-time"].as<double>();

	if (vm.count("env-map-path"))
		envMapPath = vm["env-map-path"].as<std::string>();
	if (vm.count("env-map-offset"))
		envMapOffset = vm["env-map-offset"].as<double>();
	if (vm.count("env-map-scale"))
		envMapScale = vm["env-map-scale"].as<double>();
	if (vm.count("bg-color-r"))
		bgColor.r = vm["bg-color-r"].as<double>();
	if (vm.count("bg-color-g"))
		bgColor.g = vm["bg-color-g"].as<double>();
	if (vm.count("bg-color-b"))
		bgColor.b = vm["bg-color-b"].as<double>();

	ParseOptions(vm);

	return true;
}

// --------------------------------------------------------------------------------

Renderer::Renderer( const std::shared_ptr<RendererConfig>& config )
	: commonConfig(config)
	, finishedTasks(0)
	, waitingThreads(0)
	, image(new Image(config->width, config->height))
	, scene(
		config->fixedScene
			? static_cast<Scene*>(new CornellBoxScene((double)config->width / config->height))
			: static_cast<Scene*>(new BVHScene(config->scenePath)))
{

}

Renderer::~Renderer()
{

}

void Renderer::Render()
{
	Preprocess();

	// --------------------------------------------------------------------------------

	// Create threads
	std::vector<std::thread> threads;

	for (int i = 0; i < commonConfig->numThreads; i++)
	{
		auto param = Create_Thread_InitParam(i);
		param->id = i;
		threads.push_back(std::thread(&Renderer::ProcessThread, this, param));
	}

	// --------------------------------------------------------------------------------

	// Timestamp
	auto start = std::chrono::high_resolution_clock::now();
	auto time = std::chrono::high_resolution_clock::to_time_t(start);
	double nextImageSaveTime = commonConfig->imageSaveIntervalTime;
	int totalImageSaves = 0;

	std::string timeStamp;
	std::stringstream ss;

#ifdef HINATA_PLATFORM_WINDOWS
	struct tm timeinfo;
	localtime_s(&timeinfo, &time);
	ss << std::put_time(&timeinfo, "%Y.%m.%d.%H.%M.%S");
#else
	ss << std::put_time(std::localtime(&time), "%Y.%m.%d.%H.%M.%S");
#endif

	timeStamp = ss.str();

	// --------------------------------------------------------------------------------

	// Render loop
	int pass = 0;
	double elapsed = 0;

	while (elapsed < commonConfig->executionTime)
	{
		if (!commonConfig->quiet)
			std::cerr << "Pass #" << pass << std::endl;

		// --------------------------------------------------------------------------------

		// Dispatch render tasks
		finishedTasks = 0;

		// Enqueue tasks
		for (int y = 0; y < commonConfig->numRenderTasks; y++)
		{
			Task task;
			task.command = Command::Render;
			task.needSync = false;
			queue.Enqueue(task);
		}

		// Wait for tasks
		{
			std::unique_lock<std::mutex> lock(taskFinishedMutex);

			taskFinished.wait(lock,
				[this]{
					// Print progress
					if (!commonConfig->quiet)
					{
						std::cerr <<
							(boost::format("\r  Progress : %.2lf %%")
							% ((double)finishedTasks / commonConfig->numRenderTasks * 100.0)).str();
					}
					return finishedTasks == commonConfig->numRenderTasks;
			});

			if (!commonConfig->quiet)
				std::cerr << std::endl;
		}

		// --------------------------------------------------------------------------------

		// Gather image buffers
		finishedTasks = 0;

		for (int i = 0; i < commonConfig->numThreads; i++)
		{
			Task task;
			task.command = Command::UpdateImage;
			task.needSync = true;
			queue.Enqueue(task);
		}

		{
			std::unique_lock<std::mutex> lock(taskFinishedMutex);
			taskFinished.wait(lock, [this]{ return finishedTasks == commonConfig->numThreads; });
		}

		// --------------------------------------------------------------------------------

		// Print elapsed time
		auto now = std::chrono::high_resolution_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() / 1000.0;			

		if (!commonConfig->quiet)
		{
			std::cerr << (boost::format("  Elapsed time : %.2lf seconds") % elapsed).str() << std::endl;
		}

		// --------------------------------------------------------------------------------

		pass++;
		RenderPassFinished();

		// Save image
		if (elapsed > nextImageSaveTime - Eps)
		{
			namespace fs = boost::filesystem;

			totalImageSaves++;
			nextImageSaveTime += commonConfig->imageSaveIntervalTime;

			fs::path outputDir(commonConfig->outputDir);

			if (!fs::exists(outputDir))
			{
				fs::create_directories(outputDir);
			}

			auto prefix =
				commonConfig->outputFilePrefix == ""
				? commonConfig->appName + "-" + timeStamp
				: commonConfig->outputFilePrefix;

			auto suffix =
				commonConfig->disableOutputFileSuffix
				? ""
				: "-" + (boost::format("%d-%.2lf") % totalImageSaves % elapsed).str();

			auto fileName = prefix + suffix + ".ppm";
			auto path = outputDir / fileName;

			if (!commonConfig->quiet)
			{
				std::cerr << "  Saving image : " << path << std::endl;
			}

			image->Save(path.string(), ImageSaveWeight());
			SaveImageFinished();
		}	

		if (!commonConfig->quiet)
		{
			std::cerr << std::endl;
		}
	}

	// --------------------------------------------------------------------------------

	queue.SetDone();

	for (auto& thread : threads)
	{
		thread.join();
	}
}

void Renderer::ProcessThread( std::shared_ptr<Thread_InitParam> param )
{
	Task task;

	auto shared = Create_Thread_SharedData();

	shared->rng = std::make_shared<Random>((unsigned long)std::time(nullptr) + param->id);
	shared->color.assign(commonConfig->width * commonConfig->height, Vec3d());

	threadSharedData.push_back(shared);

	InitializeThread(param, shared);

	while (!queue.Done())
	{
		queue.Dequeue(task);

		if (queue.Done())
		{
			break;
		}

		switch (task.command)
		{
		case Command::Render:
			{
				ProcessThread_Render(shared);
				break;
			}

		case Command::UpdateImage:
			{
				image->Accumulate(Vec4i(0, 0, commonConfig->width, commonConfig->height), shared->color);
				shared->color.assign(commonConfig->width * commonConfig->height, Vec3d());
				break;
			}
		}

		// Barrier
		if (task.needSync)
		{
			std::unique_lock<std::mutex> lock(taskSyncMutex);

			if (++waitingThreads == commonConfig->numThreads)
			{
				waitingThreads = 0;
				taskSync.notify_all();
			}
			else
			{
				taskSync.wait(lock);
			}
		}

		{
			std::unique_lock<std::mutex> lock(taskFinishedMutex);
			finishedTasks++;
			taskFinished.notify_one();
		}
	}
}

HINATA_NAMESPACE_END