#include "Isotropic.h"

Isotropic::Isotropic(const Color& albedo):
	DefaultingTexturalMaterial(albedo)
{

}

Isotropic::Isotropic(const Texture* albedo):
	DefaultingTexturalMaterial(albedo)
{

}

bool Isotropic::scatter(ScatterResult& scatterResult, const Ray& ray, const HitResult& hitResult) const
{
	Color albedo = texture->getTextureColor(hitResult.hitPoint, hitResult.u, hitResult.v);

	Ray scatteredRay = Ray(hitResult.hitPoint, Vector3D::randomInUnitSphere(), ray.getTime());

	scatterResult = ScatterResult(scatteredRay, albedo);
	
	return true;
}
