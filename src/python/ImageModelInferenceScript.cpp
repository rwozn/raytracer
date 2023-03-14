#include "ImageModelInferenceScript.h"

ImageModelInferenceScript::ImageModelInferenceScript(const std::string& modelFamilyName):
	PythonScript("image_model/model_inference.py")
{
	addModuleSearchPath(modelFamilyName, true);
}