#include "../Utils.h"
#include "ConstantDensityMedium.h"

#include <cmath>

ConstantDensityMedium::ConstantDensityMedium(const Hittable* boundary, float density, const Color& phaseFunction):
	DefaultingMaterialHittable(phaseFunction),
	boundary(boundary)
{
	setDensity(density);
}

ConstantDensityMedium::ConstantDensityMedium(const Hittable* boundary, float density, const Material* phaseFunction):
	DefaultingMaterialHittable(phaseFunction),
	boundary(boundary)
{
	setDensity(density);
}

float ConstantDensityMedium::getDensity() const
{
	return -1 / negativeInverseDensity;
}

const Hittable* ConstantDensityMedium::getBoundary() const
{
	return boundary;
}

void ConstantDensityMedium::setDensity(float density)
{
	negativeInverseDensity = -1 / density;
}

void ConstantDensityMedium::setBoundary(const Hittable* boundary)
{
	this->boundary = boundary;
}

bool ConstantDensityMedium::getBoundingBox(AABB& aabb, float t0, float t1) const
{
	return boundary->getBoundingBox(aabb, t0, t1);
}

bool ConstantDensityMedium::hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const
{
	HitResult firstHitResult;

	if(!boundary->hit(firstHitResult, ray, -Utils::Math::infinity, Utils::Math::infinity))
		return false;

	HitResult secondHitResult;

	if(!boundary->hit(secondHitResult, ray, firstHitResult.t + 1e-4f, Utils::Math::infinity))
		return false;

	if(firstHitResult.t < tmin)
		firstHitResult.t = tmin;

	if(secondHitResult.t > tmax)
		secondHitResult.t = tmax;

	if(firstHitResult.t >= secondHitResult.t)
		return false;

	if(firstHitResult.t < 0)
		firstHitResult.t = 0;

	const float rayLength = ray.getDirection().length();
	const float distanceInsideBoundary = (secondHitResult.t - firstHitResult.t) * rayLength;
	const float hitDistance = negativeInverseDensity * logf(Utils::randomFloat());

	if(hitDistance > distanceInsideBoundary)
		return false;

	float t = firstHitResult.t + hitDistance / rayLength;

	hitResult.t = t;
	hitResult.hitPoint = ray.at(t);

	// normal and frontFacing are arbitrary
	hitResult.frontFacing = true;
	hitResult.hitMaterial = material;
	hitResult.hitNormal = Vector3D::ofStandardBasis(Vector3D::X);

	return true;
}
