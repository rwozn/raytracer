#include "Movable.h"

Movable::Movable(const Vector3D& startOrigin, const Vector3D& endOrigin, float startTime, float endTime, const Color& color):
	StaticHittable(startOrigin, color),
	endOrigin(endOrigin),
	startTime(startTime),
	endTime(endTime)
{

}

Movable::Movable(const Vector3D& startOrigin, const Vector3D& endOrigin, float startTime, float endTime, const Material* material):
	StaticHittable(startOrigin, material),
	endOrigin(endOrigin),
	startTime(startTime),
	endTime(endTime)
{

}

bool Movable::isMoving() const
{
	return startTime != endTime;
}

float Movable::getEndTime() const
{
	return endTime;
}

float Movable::getStartTime() const
{
	return startTime;
}

void Movable::setEndTime(float time)
{
	endTime = time;
}

void Movable::setStartTime(float time)
{
	startTime = time;
}

void Movable::setEndOrigin(const Vector3D& origin)
{
	endOrigin = origin;
}

Vector3D Movable::getEndOrigin() const
{
	return endOrigin;
}

Vector3D Movable::getOrigin(float time) const
{
	if(!isMoving())
		return endOrigin;

	return origin + (endOrigin - origin) * ((time - startTime) / (endTime - startTime));
}
