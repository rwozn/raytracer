#include "Rotation.h"
#include "../Utils.h"

#include <cmath>

void Rotation::updateAABB()
{
	hasAABB = subject->getBoundingBox(aabb, 0, 1);

	const Vector3D min = aabb.getMin();
	const Vector3D max = aabb.getMax();

	static constexpr size_t ITERATIONS = 2;

	Vector3D newMin = Vector3D::filled(Utils::Math::infinity);
	Vector3D newMax = Vector3D::filled(-Utils::Math::infinity);

	for(size_t i = 0; i < ITERATIONS; i++)
		for(size_t j = 0; j < ITERATIONS; j++)
			for(size_t k = 0; k < ITERATIONS; k++)
			{
				float x = i * max.x + (1 - i) * min.x;
				float y = j * max.y + (1 - j) * min.y;
				float z = k * max.z + (1 - k) * min.z;

				Vector3D tester = Vector3D(x, y, z).rotated(axis, angle);

				for(Vector3D::Axis l = Vector3D::X; l <= Vector3D::Z; l++)
				{
					float testerValue = tester[l];

					float& newMinValue = newMin[l];
					float& newMaxValue = newMax[l];

					newMinValue = fmin(newMinValue, testerValue);
					newMaxValue = fmax(newMaxValue, testerValue);
				}
			}

	aabb = AABB(newMin, newMax);
}

Rotation::Rotation(const Hittable* subject, Vector3D::Axis axis, float angle):
	Transformation(subject),
	axis(axis)
{
	setAngle(angle);
}

float Rotation::getAngle() const
{
	return angle;
}

Vector3D::Axis Rotation::getAxis() const
{
	return axis;
}

void Rotation::setAngle(float angle)
{
	this->angle = Utils::Math::deg2rad(angle);

	updateAABB();
}

void Rotation::setAxis(Vector3D::Axis axis)
{
	this->axis = axis;

	updateAABB();
}

void Rotation::setSubject(const Hittable* hittable)
{
	Transformation::setSubject(hittable);

	updateAABB();
}

bool Rotation::getBoundingBox(AABB& aabb, float t0, float t1) const
{
	aabb = this->aabb;

	return hasAABB;
}

bool Rotation::hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const
{
	Vector3D rotatedOrigin = ray.getOrigin().rotated(axis, -angle);
	Vector3D rotatedDirection = ray.getDirection().rotated(axis, -angle);

	Ray rotatedRay(rotatedOrigin, rotatedDirection, ray.getTime());

	if(!subject->hit(hitResult, rotatedRay, tmin, tmax))
		return false;

	Vector3D rotatedNormal = hitResult.hitNormal.rotated(axis, angle);
	Vector3D rotatedHitPoint = hitResult.hitPoint.rotated(axis, angle);

	hitResult.hitPoint = rotatedHitPoint;
	hitResult.setHitNormal(rotatedRay, rotatedNormal);

	return true;
}
