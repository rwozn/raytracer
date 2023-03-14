#pragma once

#include "Transformation.h"

class FaceFlip: public Transformation
{
public:
	FaceFlip(const Hittable* hittable);

	virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const override;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const override;
};
