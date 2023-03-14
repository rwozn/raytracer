#pragma once

#include "../Image.h"
#include "ImageModelInferenceScript.h"

class ImageModel
{
protected:
	std::string modelName;
	std::string modelPath;
	std::string modelFamilyName;

	const ImageModelInferenceScript modelInferenceScript;

public:
	ImageModel(const std::string& modelFamilyName, const std::string& modelName);
	
	std::string getModelName() const;
	std::string getModelPath() const;
	std::string getModelFamilyName() const;

	Image predict(const std::string& inputImagePath, const std::string& outputImageSavePath) const;
};
