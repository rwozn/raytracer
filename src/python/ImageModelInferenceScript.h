#pragma once

#include "PythonScript.h"

class ImageModelInferenceScript: public PythonScript
{
public:
	ImageModelInferenceScript(const std::string& modelFamilyName);
};
