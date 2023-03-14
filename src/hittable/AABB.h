#pragma once

#include "../Ray.h"

class AABB
{
	Vector3D min, max;

public:
	AABB();
	AABB(const Vector3D& min, const Vector3D& max);

	Vector3D getMin() const;
	Vector3D getMax() const;

	AABB getSurroundingBox(const AABB& other) const;

	bool hit(const Ray& ray, float tmin, float tmax) const;
};

