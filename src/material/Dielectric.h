#pragma once

#include "Material.h"

class Dielectric: public Material
{
	float refractionIndex;

	// Use Schlick's approximation for reflectance
	static float reflectance(float cosTheta, float refractionRatio);

public:
	Dielectric(float refractionIndex);

	float getRefractionIndex() const;
	void setRefractionIndex(float refractionIndex);
	
	virtual bool scatter(ScatterResult& scatterResult, const Ray& ray, const HitResult& hitResult) const override;
};

