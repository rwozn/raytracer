#pragma once

#include "Hittable.h"

class MaterialHittable: public Hittable
{
protected:
	const Material* material;
	
public:
	MaterialHittable(const Material* material);
	virtual ~MaterialHittable();

	const Material* getMaterial() const;
	virtual void setMaterial(const Material* material);
};
