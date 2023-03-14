#pragma once

#include "../material/Isotropic.h"
#include "DefaultingMaterialHittable.h"

// Note: works only with boundaries whose shape is convex,
// e.g. boxex and spheres, but not with toruses
class ConstantDensityMedium: public DefaultingMaterialHittable<Isotropic>
{
	const Hittable* boundary;

	float negativeInverseDensity;

public:
	ConstantDensityMedium(const Hittable* boundary, float density, const Color& phaseFunction);
	ConstantDensityMedium(const Hittable* boundary, float density, const Material* phaseFunction);

	float getDensity() const;
	const Hittable* getBoundary() const;

	void setDensity(float density);
	void setBoundary(const Hittable* boundary);

	virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const override;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const override;
};

