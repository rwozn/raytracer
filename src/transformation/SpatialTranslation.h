#pragma once

#include "Transformation.h"

class SpatialTranslation: public Transformation
{
	Vector3D offset;

public:
	SpatialTranslation(const Hittable* subject, const Vector3D& offset);
	
	Vector3D getOffset() const;
	void setOffset(const Vector3D& offset);

	virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const override;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const override;
};

