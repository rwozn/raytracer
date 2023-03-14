#include "../Utils.h"
#include "Dielectric.h"

#include <cmath>

float Dielectric::reflectance(float cosTheta, float refractionRatio)
{
	float r = (1 - refractionRatio) / (1 + refractionRatio);

	r = r * r;

	return r + (1 - r) * pow(1 - cosTheta, 5.f);
}

Dielectric::Dielectric(float refractionIndex):
	refractionIndex(refractionIndex)
{

}

float Dielectric::getRefractionIndex() const
{
	return refractionIndex;
}

void Dielectric::setRefractionIndex(float refractionIndex)
{
	this->refractionIndex = refractionIndex;
}

bool Dielectric::scatter(ScatterResult& scatterResult, const Ray& ray, const HitResult& hitResult) const
{
	const float refractionRatio = hitResult.frontFacing ? 1 / refractionIndex : refractionIndex;

	Vector3D unitDirection = ray.getDirection().normalized();

	const float cosTheta = fmin((-unitDirection).dotProduct(hitResult.hitNormal), 1.f);
	const float sinTheta = sqrt(1 - cosTheta * cosTheta);

	bool canRefract = refractionRatio * sinTheta <= 1;

	Vector3D refracted = canRefract && reflectance(cosTheta, refractionRatio) <= Utils::randomFloat() ? unitDirection.refract(hitResult.hitNormal, refractionRatio) : unitDirection.reflect(hitResult.hitNormal);

	scatterResult = ScatterResult(Ray(hitResult.hitPoint, refracted, ray.getTime()), Color::WHITE);

	return true;
}
