#pragma once

#include "DefaultingMaterialHittable.h"

class Rectangle: public DefaultingMaterialHittable<>
{
	AABB aabb;

	float k;
	float a0, a1;
	float b0, b1;

	Vector3D::Axis axis;

	// e.g. XY rectangle has:
	// axisA=X, axisB=Y; a0=x0, a1=x1; b0=y0, b1=y1
	Vector3D::Axis axisA, axisB;

	void setBounds(float a0, float a1, float b0, float b1, float k, Vector3D::Axis axis);

public:
	// k is for axis, (e.g. XY rectangle has k for Z)
	Rectangle(float a0, float a1, float b0, float b1, float k, Vector3D::Axis axis, const Color& color);
	Rectangle(float a0, float a1, float b0, float b1, float k, Vector3D::Axis axis, const Material* material);
	
	virtual Vector3D getRandomPoint(const Vector3D& origin) const override;
	virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const override;
	virtual float getPdfValue(const Vector3D& origin, const Vector3D& direction) const override;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const override;
};

