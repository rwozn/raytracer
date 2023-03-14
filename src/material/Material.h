#pragma once

#include "../Ray.h"
#include "../hittable/ScatterResult.h"
#include "../hittable/HitResult.h"

struct HitResult;

class Material
{
public:
	virtual ~Material();

	virtual bool scatter(ScatterResult& scatterResult, const Ray& ray, const HitResult& hitResult) const;
	virtual float getScatteringPdf(const Ray& ray, const HitResult& hitResult, const Ray& scatteredRay) const;
	virtual Color emitColor(const Ray& ray, const HitResult& hitResult, const Vector3D& point, float u, float v) const;
};
