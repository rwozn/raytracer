#include "SceneWindow.h"

#include "../python/models/deepdream/DeepDreamImageModel.h"
#include "../python/models/maxim/MaximImageDenoisingModel.h"
#include "../python/models/esrgan/EsrganImageEnhancingModel.h"
#include "../python/models/styletransfer/StyleTransferImageModel.h"

#include <cmath>
#include <mutex>
#include <omp.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <filesystem>

template <class Rep, class Period>
static std::string formatTime(const std::chrono::duration<Rep, Period>& duration)
{
	using std::chrono::duration_cast;

	auto hours = duration_cast<std::chrono::hours>(duration);
	auto minutes = duration_cast<std::chrono::minutes>(duration - hours);
	auto seconds = duration_cast<std::chrono::seconds>(duration - hours - minutes);

	auto h = hours.count();
	auto min = minutes.count();
	auto s = seconds.count();

	std::string formattedTime = std::to_string(s) + 's';

	if(min > 0)
		formattedTime = std::to_string(min) + "min " + formattedTime;

	if(h > 0)
		formattedTime = std::to_string(h) + "h " + formattedTime;

	return formattedTime;
}

static void addFlagString(std::string& formattedFlags, const std::string& flag)
{
	if(!formattedFlags.empty())
		formattedFlags += " | ";

	formattedFlags += flag;
}

std::string SceneWindow::postCreationActionFlagsToString(size_t flags)
{
	if(!flags)
		return "none";

	if(flags == SAVE)
		return "save";

	std::string formattedFlags;

	if(flags & DENOISE)
		addFlagString(formattedFlags, "denoise");

	if(flags & STYLE_TRANSFER)
		addFlagString(formattedFlags, "style transfer");

	if(flags & DREAM)
		addFlagString(formattedFlags, "dream");

	if(flags & ENHANCE)
		addFlagString(formattedFlags, "enhance");

	return formattedFlags;
}

std::string SceneWindow::drawingModeToString(SceneDrawingMode drawingMode)
{
	switch(drawingMode)
	{
		case SceneDrawingMode::PIXEL_BY_PIXEL: return "pixel by pixel";
		case SceneDrawingMode::WHEN_REPORTING: return "during progress report";
		case SceneDrawingMode::WHEN_FINISHED: return "when ray tracing is done";
	}

	throw std::runtime_error("Invalid scene drawing mode");
}

std::string SceneWindow::getSaveFileDirectoryPath(const std::string& saveFileDirectoryName) const
{
	return saveDirectoryPath + '/' + saveFileDirectoryName;
}

std::string SceneWindow::getSaveFilePath(const std::string& saveFileDirectoryName, const std::string& saveFileName) const
{
	return getSaveFileDirectoryPath(saveFileDirectoryName) + '/' + saveFileName + ".png";
}

using std::chrono::steady_clock;

std::string SceneWindow::getSaveFileDirectoryName(const steady_clock::duration& rayTracingDuration, size_t workerThreadsCount) const
{
	size_t maxDepth = rayTracer.getMaxDepth();
	size_t samplesPerPixel = rayTracer.getSamplesPerPixel();

	const Camera* camera = rayTracer.getCamera();

	float fov = camera->getFov();

	std::ostringstream ss;
	
	ss << rayTracer.getScene()->getName() << "_" << width << 'x' << height << '_'
	   << samplesPerPixel << "spp_"
	   << maxDepth << "maxdepth_"
	   << fov << "fov";

	float shutterOpenTime = camera->getShutterOpenTime();
	float shutterCloseTime = camera->getShutterCloseTime();

	if(shutterOpenTime || shutterCloseTime)
		ss << '_' << shutterOpenTime << "shutteropen_" << shutterCloseTime << "shutterclose";

	if(workerThreadsCount > 1)
		ss << '_' << workerThreadsCount << "workers";

	ss << '_' << formatTime(rayTracingDuration);

	return ss.str();
}

void SceneWindow::reportRayTracingProgress(float progress, const steady_clock::duration& rayTracingDuration, steady_clock::duration& previousRayTracingDuration)
{
	std::ostringstream ss;
	
	// 4 because e.g. 75.25% - 4 digits
	ss << std::setprecision(4) << progress * 100 << '%';

	// ETA and time delta doesn't include drawing etc. (which could make a difference if e.g. drawing pixel by pixel),
	// it only regards ray tracing duration
	std::cout << "\t-> ";
	std::cout << std::left << std::setw(6) << ss.str(); // align % (6 because e.g. "95.55%".length=6)
	std::cout << " | " << std::setw(9) << formatTime(rayTracingDuration - previousRayTracingDuration); // align time (9 because e.g. "55min 40s".length=9)
	std::cout << " | ETA: " << formatTime(rayTracingDuration * (1 / progress - 1)) << std::endl;

	previousRayTracingDuration = rayTracingDuration;
}

SceneWindow::SceneWindow(size_t width, const RayTracer& rayTracer, const std::string& saveDirectoryPath):
	DrawingWindow(width, width / rayTracer.getCamera()->getAspectRatio(), "Ray tracer (" + rayTracer.getScene()->getName() + ")"),
	rayTracer(rayTracer),
	saveDirectoryPath(saveDirectoryPath)
{

}

std::string SceneWindow::getSaveDirectoryPath() const
{
	return saveDirectoryPath;
}

void SceneWindow::setSaveDirectoryPath(const std::string& path)
{
	saveDirectoryPath = path;
}

static std::string getOutputImageFileName(const std::string& lastOutputImageFileName, const ImageModel& imageModel, const std::string& description)
{
	std::string outputImageFileName = lastOutputImageFileName;

	if(!outputImageFileName.empty())
		outputImageFileName += '&';

	outputImageFileName += imageModel.getModelFamilyName() + '_' + description;

	return outputImageFileName;
}

static std::string captializedFirstLetter(const std::string& str)
{
	std::string copy = str;

	copy[0] = toupper(copy[0]);

	return copy;
}

void SceneWindow::modifyImage(Image& image, const ImageModel& model, std::string& inputImagePath, std::string& lastOutputImageFileName, const std::string& saveFileDirectoryName, const std::string& description, const std::string& operation) const
{
	std::string outputImageFileName = getOutputImageFileName(lastOutputImageFileName, model, description);

	std::string outputImagePath = getSaveFilePath(saveFileDirectoryName, outputImageFileName);

	std::cout << "-> " << captializedFirstLetter(description) << " image will be saved as \"" << outputImagePath << "\"" << std::endl;

	std::cout << captializedFirstLetter(operation) << " image using \"" << model.getModelPath() << "\" model..." << std::endl;

	steady_clock::time_point start = steady_clock::now();

	image = model.predict(inputImagePath, outputImagePath);

	steady_clock::time_point end = steady_clock::now();

	std::cout << "Finished " << operation << " in " << formatTime(end - start) << std::endl;

	pollEvents();
	drawImage(image);

	inputImagePath = outputImagePath;
	lastOutputImageFileName = outputImageFileName;
}

Image SceneWindow::drawScene(float progressReportingFrequency, SceneDrawingMode drawingMode, size_t flags) const
{
	// set background color as the default color so that when a not finished image
	// is redrawn its background matches the window's background color
	Image image(width, height, backgroundColor);
	
	const size_t width = image.getWidth();
	const size_t height = image.getHeight();
	
	std::cout << "Creating image... (" << rayTracer.getScene()->getName() << ')' << std::endl;
	
	std::cout << "-> " << width << "x" << height << " (" << rayTracer.getSamplesPerPixel() << " samples ppx)" << std::endl;
	std::cout << "-> reporting progress every " << progressReportingFrequency * 100 << '%' << std::endl;
	std::cout << "-> drawing " << drawingModeToString(drawingMode) << std::endl;
	std::cout << "-> post creation flags: " << postCreationActionFlagsToString(flags) << std::endl;
	
	struct ColoredPixel
	{
		Color color;
		size_t x, y;

		ColoredPixel(size_t x, size_t y, const Color& color):
			color(color),
			x(x),
			y(y)
		{

		}
	};
	
	size_t workerThreadsCount = 0;
	
	std::mutex coloredPixelsMutex;
	std::mutex rayTracingDurationMutex;
	
	std::vector<ColoredPixel> coloredPixels;
	
	const size_t pixelsCount = image.getSize();

	steady_clock::duration rayTracingDuration(0);
	
	const steady_clock::time_point totalStart = steady_clock::now();
	
	// https://bisqwit.iki.fi/story/howto/openmp/
	// start parallel block - create a team of threads
	#pragma omp parallel
	{
		// run the loop on all threads except the master, which will be drawing and reporting progress
		#pragma omp master // the following code gets executed by the master thread only
		{
			workerThreadsCount = omp_get_num_threads() - 1;

			std::cout << "-> " << workerThreadsCount << " worker threads" << std::endl;

			size_t coloredPixelsCount = 0;

			steady_clock::duration previousRayTracingDuration = rayTracingDuration;

			const size_t progressReportCheckpoint = round(pixelsCount * progressReportingFrequency);

			// index of the farthest pixel that has been colored by a worker thread
			// (they might be not colored sequentially, e.g. (0,1), (0,2), (0,4), (0,3)...)
			size_t farthestColoredPixelIndex = 0;

			// continue until all pixels have been colored
			while(coloredPixelsCount != pixelsCount)
			{
				// poll events to keep the window responsive
				pollEvents();

				// move colored pixels so that other worker threads don't wait
				// for the mutex to be unlocked while we're handling them, e.g. during drawing
				coloredPixelsMutex.lock();
				std::vector<ColoredPixel> pixelsToUpdate(std::move(coloredPixels));
				coloredPixelsMutex.unlock();

				for(const ColoredPixel& coloredPixel: pixelsToUpdate)
				{
					coloredPixelsCount++;

					const size_t x = coloredPixel.x;
					const size_t y = coloredPixel.y;

					image.setPixel(x, y, coloredPixel.color);

					const size_t coloredPixelIndex = x * height + y;

					// update the farthest colored pixel index so that the entire image doesn't
					// have to be redrawn every time but only until the farthest already colored pixel
					if(coloredPixelIndex > farthestColoredPixelIndex)
						farthestColoredPixelIndex = coloredPixelIndex;

					if(drawingMode == SceneDrawingMode::PIXEL_BY_PIXEL)
						drawImagePart(image, 0, farthestColoredPixelIndex);

					// report progress with specified frequency or if it's the last pixel
					// (in case frequency couldn't fit evenly, e.g. size=100, frequency=0.3)
					if(coloredPixelsCount == pixelsCount || coloredPixelsCount % progressReportCheckpoint == 0)
					{
						if(drawingMode == SceneDrawingMode::WHEN_REPORTING)
							drawImagePart(image, 0, farthestColoredPixelIndex);

						rayTracingDurationMutex.lock();
						steady_clock::duration duration = rayTracingDuration;
						rayTracingDurationMutex.unlock();

						// don't do just `progress = (float)coloredPixelsCount / pixelsCount` because it causes
						// minor floating point division errors at times (e.g. 5.101% instead of 5.1%)
						// (`coloredPixelsCount % progressReportCheckpoint == 0` so `coloredPixelsCount / progressReportCheckpoint` returns an int)
						float progress = coloredPixelsCount == pixelsCount ? 1.f : progressReportingFrequency * (coloredPixelsCount / progressReportCheckpoint);

						reportRayTracingProgress(progress, duration / workerThreadsCount, previousRayTracingDuration);
					}
				}
			}

			rayTracingDuration /= workerThreadsCount;
		}
		
		// the following code gets executed by worker threads only
		/*
			In the dynamic schedule, there is no predictable order in which the loop
			items are assigned to different threads. Each thread asks the OpenMP
			runtime library for an iteration number, then handles it, then asks for next,
			and so on. This is most useful when used in conjunction with the ordered clause,
			or when the different iterations in the loop may take different time to execute.
		*/
		#pragma omp for schedule(dynamic)
		for(int i = 0; i < pixelsCount; i++)
		{
			size_t x = i / height;
			size_t y = i % height;

			steady_clock::time_point start = steady_clock::now();

			Color color = rayTracer.getColor(x, y, width, height);

			// don't count anything besides the rayTracer call
			steady_clock::time_point end = steady_clock::now();

			steady_clock::duration duration = end - start;
			
			rayTracingDurationMutex.lock();
			rayTracingDuration += duration;
			rayTracingDurationMutex.unlock();

			coloredPixelsMutex.lock();
			coloredPixels.push_back(ColoredPixel(x, y, color));
			coloredPixelsMutex.unlock();
		}
	}
	
	const steady_clock::time_point totalEnd = steady_clock::now();
	
	const steady_clock::duration totalDuration = totalEnd - totalStart;

	if(drawingMode == SceneDrawingMode::WHEN_FINISHED)
		drawImage(image);
	
	std::cout << "Finished ray tracing in " << formatTime(rayTracingDuration) << " (total time: " << formatTime(totalDuration) << " -> " << formatTime(totalDuration - rayTracingDuration) << " overhead)" << std::endl;

	if(!flags)
		return image;
	
	const std::string saveFileDirectoryName = getSaveFileDirectoryName(rayTracingDuration, workerThreadsCount);

	// create the save directories
	std::filesystem::create_directories(getSaveFileDirectoryPath(saveFileDirectoryName));
	
	if(flags == SAVE)
	{
		std::string path = getSaveFilePath(saveFileDirectoryName, "result");

		image.save(path);

		std::cout << "Saved as \"" << path << "\"" << std::endl;

		return image;
	}

	std::string inputImagePath = getSaveFilePath(saveFileDirectoryName, "raw_result");

	image.save(inputImagePath);
	
	std::cout << "-> Created image has been saved as \"" << inputImagePath << "\"" << std::endl;
	
	std::string lastOutputImageFileName;

#define MODIFY_IMAGE(MODEL_CLASS, DESCRIPTION, OPERATION)\
	modifyImage(image, MODEL_CLASS(), inputImagePath, lastOutputImageFileName, saveFileDirectoryName, DESCRIPTION, OPERATION);\

	if(flags & DENOISE)
		MODIFY_IMAGE(MaximImageDenoisingModel, "denoised", "denoising");

	if(flags & STYLE_TRANSFER)
		MODIFY_IMAGE(StyleTransferImageModel, "style transferred", "style transferring");

	if(flags & DREAM)
		MODIFY_IMAGE(DeepDreamImageModel, "dreamed", "dreaming");

	if(flags & ENHANCE)
		MODIFY_IMAGE(EsrganImageEnhancingModel, "enhanced", "enhancing");

	return image;
}
