#include "HitResult.h"

HitResult::HitResult():
	t(0),
	u(0),
	v(0),
	frontFacing(false),
	hitObject(nullptr),
	hitMaterial(nullptr)
{

}

HitResult::HitResult(const Ray& ray, const Vector3D& hitPoint, const Vector3D& outwardNormal, const Hittable* hitObject, const Material* hitMaterial, float t, float u, float v):
	t(t),
	u(u),
	v(v),
	hitPoint(hitPoint),
	hitObject(hitObject),
	hitMaterial(hitMaterial)
{
	setHitNormal(ray, outwardNormal);
}

void HitResult::setHitNormal(const Ray& ray, const Vector3D& outwardNormal)
{
	frontFacing = ray.getDirection().dotProduct(outwardNormal) < 0;
	hitNormal = frontFacing ? outwardNormal : -outwardNormal;
}