#include "ImageModel.h"

ImageModel::ImageModel(const std::string& modelFamilyName, const std::string& modelName):
    modelName(modelName),
    modelPath(modelFamilyName + '/' + modelName),
    modelFamilyName(modelFamilyName),
    modelInferenceScript(modelFamilyName)
{

}

std::string ImageModel::getModelName() const
{
    return modelName;
}

std::string ImageModel::getModelPath() const
{
    return modelPath;
}

std::string ImageModel::getModelFamilyName() const
{
    return modelFamilyName;
}

Image ImageModel::predict(const std::string& inputImagePath, const std::string& outputImagePath) const
{
    modelInferenceScript.runScript({modelFamilyName, modelName, inputImagePath, outputImagePath});

    return Image(outputImagePath);
}
