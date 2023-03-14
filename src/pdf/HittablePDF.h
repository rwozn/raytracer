#pragma once

#include "PDF.h"
#include "../hittable/Hittable.h"

class HittablePDF: public PDF
{
	Vector3D origin;

	const Hittable* hittable;

public:
	HittablePDF(const Hittable* hittable, const Vector3D& origin);

	void setOrigin(const Vector3D& origin);
	void setHittable(const Hittable* hittable);

	Vector3D getOrigin(const Vector3D& origin) const;
	const Hittable* getHittable(const Hittable* hittable) const;

	virtual Vector3D generateVector() const override;
	virtual float getValue(const Vector3D& direction) const override;
};
