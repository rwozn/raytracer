#include "AABB.h"

AABB::AABB()
{

}

AABB::AABB(const Vector3D& min, const Vector3D& max):
	min(min),
	max(max)
{
	
}

Vector3D AABB::getMin() const
{
	return min;
}

Vector3D AABB::getMax() const
{
	return max;
}

AABB AABB::getSurroundingBox(const AABB& other) const
{
	return AABB(min.min(other.min), max.max(other.max));
}

bool AABB::hit(const Ray& ray, float tmin, float tmax) const
{
	Vector3D origin = ray.getOrigin();
	Vector3D direction = ray.getDirection();

	for(Vector3D::Axis i = Vector3D::X; i <= Vector3D::Z; i++)
	{
		float inverseDirection = 1 / direction[i];
		
		float t0 = (min[i] - origin[i]) * inverseDirection;
		float t1 = (max[i] - origin[i]) * inverseDirection;

		if(inverseDirection < 0)
			std::swap(t0, t1);

		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;

		if(tmax <= tmin)
			return false;
	}

	return true;
}