#include "Hittable.h"

Hittable::~Hittable()
{

}

Vector3D Hittable::getRandomPoint(const Vector3D& origin) const
{
	return Vector3D::ofStandardBasis(Vector3D::X);
}

float Hittable::getPdfValue(const Vector3D& origin, const Vector3D& direction) const
{
	return 0;
}