#include "FaceFlip.h"

FaceFlip::FaceFlip(const Hittable* hittable):
	Transformation(hittable)
{

}

bool FaceFlip::getBoundingBox(AABB& aabb, float t0, float t1) const
{
	return subject->getBoundingBox(aabb, t0, t1);
}

bool FaceFlip::hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const
{
	if(!subject->hit(hitResult, ray, tmin, tmax))
		return false;

	hitResult.frontFacing = !hitResult.frontFacing;

	return true;
}
