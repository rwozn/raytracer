#pragma once

#include "DefaultingTexturalMaterial.h"

class Isotropic: public DefaultingTexturalMaterial<>
{
public:
	Isotropic(const Color& albedo);
	Isotropic(const Texture* albedo);

	virtual bool scatter(ScatterResult& scatterResult, const Ray& ray, const HitResult& hitResult) const override;
};

