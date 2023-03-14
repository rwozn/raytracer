#pragma once

#include "../Ray.h"
#include "../pdf/PDF.h"
#include "../color/Color.h"

struct ScatterResult
{
	const PDF* pdf;
	Ray specularRay;
	Color attenuation;

	ScatterResult();
	ScatterResult(const PDF* pdf, const Color& attenuation);
	ScatterResult(const Ray& specularRay, const Color& attenuation);

	bool isSpecular();
};
