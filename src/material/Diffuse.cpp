#include "Diffuse.h"
#include "../Utils.h"
#include "../pdf/CosinePDF.h"

Diffuse::Diffuse(const Color& albedo):
	DefaultingTexturalMaterial(albedo)
{

}

Diffuse::Diffuse(const Texture* albedo):
	DefaultingTexturalMaterial(albedo)
{

}

bool Diffuse::scatter(ScatterResult& scatterResult, const Ray& ray, const HitResult& hitResult) const
{
	scatterResult = ScatterResult(new CosinePDF(hitResult.hitNormal), texture->getTextureColor(hitResult.hitPoint, hitResult.u, hitResult.v));

	return true;
}

float Diffuse::getScatteringPdf(const Ray& ray, const HitResult& hitResult, const Ray& scatteredRay) const
{
	float cosine = hitResult.hitNormal.dotProduct(scatteredRay.getDirection().normalized());

	return cosine <= 0 ? 0 : cosine / Utils::Math::pi;
}
