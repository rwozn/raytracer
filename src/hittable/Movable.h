#pragma once

#include "StaticHittable.h"

class Movable: public StaticHittable
{
	Vector3D endOrigin;

	float startTime, endTime;

public:
	Movable(const Vector3D& origin, const Vector3D& endOrigin, float startTime, float endTime, const Color& color);
	Movable(const Vector3D& origin, const Vector3D& endOrigin, float startTime, float endTime, const Material* material);

	bool isMoving() const;

	float getEndTime() const;
	float getStartTime() const;

	void setEndTime(float time);
	void setStartTime(float time);
	void setEndOrigin(const Vector3D& origin);

	Vector3D getEndOrigin() const;
	Vector3D getOrigin(float time) const;
};
