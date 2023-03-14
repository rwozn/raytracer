#pragma once

#include "DefaultingTexturalMaterial.h"

class DiffuseLight: public DefaultingTexturalMaterial<>
{
public:
	DiffuseLight(const Color& emitter);
	DiffuseLight(const Texture* emitter);

	virtual Color emitColor(const Ray& ray, const HitResult& hitResult, const Vector3D& point, float u, float v) const override;
};
