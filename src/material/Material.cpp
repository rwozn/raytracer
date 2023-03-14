#include "Material.h"

Material::~Material()
{

}

bool Material::scatter(ScatterResult& scatterResult, const Ray& ray, const HitResult& hitResult) const
{
	return false;
}

float Material::getScatteringPdf(const Ray& ray, const HitResult& hitResult, const Ray& scatteredRay) const
{
	return 0;
}

Color Material::emitColor(const Ray& ray, const HitResult& hitResult, const Vector3D& point, float u, float v) const
{
	return Color::BLACK;
}
