#include "DiffuseLight.h"

DiffuseLight::DiffuseLight(const Color& emitter):
	DefaultingTexturalMaterial(emitter)
{

}

DiffuseLight::DiffuseLight(const Texture* emitter):
	DefaultingTexturalMaterial(emitter)
{

}

Color DiffuseLight::emitColor(const Ray& ray, const HitResult& hitResult, const Vector3D& point, float u, float v) const
{
	if(hitResult.frontFacing)
		return texture->getTextureColor(point, u, v);

	return Color::BLACK;
}
