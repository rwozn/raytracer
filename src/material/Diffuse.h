#pragma once

#include "DefaultingTexturalMaterial.h"

// Lambertian
class Diffuse: public DefaultingTexturalMaterial<>
{
public:
	Diffuse(const Color& albedo);
	Diffuse(const Texture* albedo);

	virtual bool scatter(ScatterResult& scatterResult, const Ray& ray, const HitResult& hitResult) const override;
	virtual float getScatteringPdf(const Ray& ray, const HitResult& hitResult, const Ray& scatteredRay) const override;
};
