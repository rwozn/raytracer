#pragma once

#include "DefaultingMaterialHittable.h"

class StaticHittable: public DefaultingMaterialHittable<>
{
protected:
	Vector3D origin;
	
public:
	StaticHittable(const Vector3D& origin, const Color& color);
	StaticHittable(const Vector3D& origin, const Material* material);

	Vector3D getOrigin() const;
	void setOrigin(const Vector3D& origin);
};
