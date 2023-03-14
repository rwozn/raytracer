#pragma once

#include "Transformation.h"

class Rotation: public Transformation
{
	AABB aabb;
	
	float angle;
	
	bool hasAABB;
	
	Vector3D::Axis axis;
	
	void updateAABB();

public:
	// angle is in degrees
	Rotation(const Hittable* subject, Vector3D::Axis axis, float angle);

	float getAngle() const;
	Vector3D::Axis getAxis() const;

	void setAngle(float angle);
	void setAxis(Vector3D::Axis axis);

	virtual void setSubject(const Hittable* hittable) override;
	virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const override;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const override;
};

