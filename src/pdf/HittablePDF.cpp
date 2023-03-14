#include "HittablePDF.h"

HittablePDF::HittablePDF(const Hittable* hittable, const Vector3D& origin):
	origin(origin),
	hittable(hittable)
{

}

void HittablePDF::setOrigin(const Vector3D& origin)
{
	this->origin = origin;
}

void HittablePDF::setHittable(const Hittable* hittable)
{
	this->hittable = hittable;
}

Vector3D HittablePDF::getOrigin(const Vector3D& origin) const
{
	return origin;
}

const Hittable* HittablePDF::getHittable(const Hittable* hittable) const
{
	return hittable;
}

Vector3D HittablePDF::generateVector() const
{
	return hittable->getRandomPoint(origin);
}

float HittablePDF::getValue(const Vector3D& direction) const
{
	return hittable->getPdfValue(origin, direction);
}
