#include "SpatialTranslation.h"

SpatialTranslation::SpatialTranslation(const Hittable* subject, const Vector3D& offset):
	Transformation(subject),
	offset(offset)
{

}

Vector3D SpatialTranslation::getOffset() const
{
	return offset;
}

void SpatialTranslation::setOffset(const Vector3D& offset)
{
	this->offset = offset;
}

bool SpatialTranslation::getBoundingBox(AABB& aabb, float t0, float t1) const
{
	if(!subject->getBoundingBox(aabb, t0, t1))
		return false;

	aabb = AABB(aabb.getMin() + offset, aabb.getMax() + offset);

	return true;
}

bool SpatialTranslation::hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const
{
	Ray movedRay(ray.getOrigin() - offset, ray.getDirection(), ray.getTime());

	if(!subject->hit(hitResult, movedRay, tmin, tmax))
		return false;

	hitResult.hitPoint += offset;
	hitResult.setHitNormal(movedRay, hitResult.hitNormal);

	return true;
}
