#pragma once

#include "Hittable.h"
#include "../material/Material.h"

class Hittable;
class Material;

struct HitResult
{
	float t;

	float u, v;

	Vector3D hitPoint;
	Vector3D hitNormal;

	bool frontFacing;
	
	const Hittable* hitObject;
	const Material* hitMaterial;

	HitResult();
	HitResult(const Ray& ray, const Vector3D& hitPoint, const Vector3D& outwardNormal, const Hittable* hitObject, const Material* hitMaterial, float t, float u, float v);

	void setHitNormal(const Ray& ray, const Vector3D& outwardNormal);
};
