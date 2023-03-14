#pragma once

#include "../Vector3D.h"

// PDF - probability density function
class PDF
{
public:
	virtual ~PDF();

	virtual Vector3D generateVector() const = 0;
	virtual float getValue(const Vector3D& direction) const = 0;
};
