#pragma once

#include "Vector3D.h"

class Ray
{
	float time;
	Vector3D origin;
	Vector3D direction;

public:
	Ray();
	Ray(const Vector3D& origin, const Vector3D& direction, float time = 0);
	
	Vector3D at(float t) const;

	float getTime() const;
	Vector3D getOrigin() const;
	Vector3D getDirection() const;
};

