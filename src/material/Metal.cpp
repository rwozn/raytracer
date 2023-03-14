#include "Metal.h"

Metal::Metal(const Color& albedo, float fuzziness):
	DefaultingTexturalMaterial(albedo)
{
	setFuzziness(fuzziness);
}

Metal::Metal(const Texture* albedo, float fuzziness):
	DefaultingTexturalMaterial(albedo)
{
	setFuzziness(fuzziness);
}

float Metal::getFuzziness() const
{
	return fuzziness;
}

void Metal::setFuzziness(float fuzziness)
{
	this->fuzziness = fuzziness < 1 ? fuzziness : 1;
}

bool Metal::scatter(ScatterResult& scatterResult, const Ray& ray, const HitResult& hitResult) const
{
	Color albedo = texture->getTextureColor(hitResult.hitPoint, hitResult.u, hitResult.v);
	
	Vector3D reflected = ray.getDirection().normalized().reflect(hitResult.hitNormal);
	
	Ray specularRay = Ray(hitResult.hitPoint, reflected + Vector3D::randomInUnitSphere() * fuzziness);
	
	scatterResult = ScatterResult(specularRay, albedo);
	
	return true;
}
