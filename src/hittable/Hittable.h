#pragma once

#include "AABB.h"
#include "HitResult.h"

struct HitResult;

class Hittable
{
public:
	virtual ~Hittable();

	virtual Vector3D getRandomPoint(const Vector3D& origin) const;
	virtual float getPdfValue(const Vector3D& origin, const Vector3D& direction) const;

	// in time interval [t0, t1]
	virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const = 0;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const = 0;
};
