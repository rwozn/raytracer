#pragma once

#include "../RayTracer.h"
#include "DrawingWindow.h"
#include "../python/ImageModel.h"

#include <chrono>

#define SET_BIT(N) (1 << (N))

class SceneWindow: public DrawingWindow
{
public:
	// drawing during ray tracing causes SDL to keep allocating a lot of memory over time (from dozens to even hundreds of MB)
	enum class SceneDrawingMode
	{
		WHEN_FINISHED,
		PIXEL_BY_PIXEL,
		WHEN_REPORTING
	};

	enum ScenePostCreationActionFlag
	{
		SAVE = SET_BIT(0),
		DENOISE = SET_BIT(1),
		ENHANCE = SET_BIT(2),
		DREAM = SET_BIT(3),
		STYLE_TRANSFER = SET_BIT(4)
	};

private:
	RayTracer rayTracer;

	std::string saveDirectoryPath;
	
	static std::string postCreationActionFlagsToString(size_t flags);
	static std::string drawingModeToString(SceneDrawingMode drawingMode);
	
	std::string getSaveFileDirectoryPath(const std::string& saveFileDirectoryName) const;
	std::string getSaveFilePath(const std::string& saveFileDirectoryName, const std::string& saveFileName) const;

	std::string getSaveFileDirectoryName(const std::chrono::steady_clock::duration& rayTracingDuration, size_t workerThreadsCount) const;

	static void reportRayTracingProgress(float progress, const std::chrono::steady_clock::duration& rayTracingDuration, std::chrono::steady_clock::duration& previousRayTracingDuration);

	void modifyImage(Image& image, const ImageModel& model, std::string& inputImagePath, std::string& lastOutputImageFileName, const std::string& saveFileDirectoryName, const std::string& description, const std::string& operation) const;

public:
	SceneWindow(size_t width, const RayTracer& rayTracer, const std::string& saveDirectoryPath = "saves");

	std::string getSaveDirectoryPath() const;
	void setSaveDirectoryPath(const std::string& path);

	// draws the scene and returns the drawn image
	Image drawScene(float progressReportingFrequency = 0.05f, SceneDrawingMode drawingMode = SceneDrawingMode::WHEN_REPORTING, size_t flags = DENOISE | STYLE_TRANSFER) const;
};
