#pragma once

#include "Rectangle.h"
#include "HittableList.h"
#include "DefaultingMaterialHittable.h"

class Box: public DefaultingMaterialHittable<>
{
	AABB aabb;

	HittableList sides;

	void addSides(Vector3D::Axis axis);

	void createBounds(const Vector3D& min, const Vector3D& max);

public:
	Box(const Vector3D& min, const Vector3D& max, const Color& color);
	Box(const Vector3D& min, const Vector3D& max, const Material* material);
	~Box();

	virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const override;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const override;
};

