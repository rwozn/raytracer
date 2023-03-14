#include "Ray.h"

Ray::Ray():
	time(0)
{

}

Ray::Ray(const Vector3D& origin, const Vector3D& direction, float time):
	time(time),
	origin(origin),
	direction(direction)
{

}

Vector3D Ray::at(float t) const
{
	return origin + direction * t;
}

float Ray::getTime() const
{
	return time;
}

Vector3D Ray::getOrigin() const
{
	return origin;
}

Vector3D Ray::getDirection() const
{
	return direction;
}
