#include "ScatterResult.h"

ScatterResult::ScatterResult():
	pdf(nullptr)
{

}

ScatterResult::ScatterResult(const PDF* pdf, const Color& attenuation):
	pdf(pdf),
	attenuation(attenuation)
{

}

ScatterResult::ScatterResult(const Ray& specularRay, const Color& attenuation):
	pdf(nullptr),
	specularRay(specularRay),
	attenuation(attenuation)
{

}

bool ScatterResult::isSpecular()
{
	return !pdf;
}