#include "../Utils.h"
#include "Rectangle.h"

#include <cmath>

void Rectangle::setBounds(float a0, float a1, float b0, float b1, float k, Vector3D::Axis axis)
{
	// the bounding must have non-zero width in each dimension,
	// so pad the `axis` dimension by a small amount
	static constexpr float PADDING = 1e-4f;

	Vector3D min = Vector3D::filledAlongAxis(a0, b0, axis, k - PADDING, axisA, axisB);

	Vector3D max = Vector3D::ofScaledStandardBasis(axis, k + PADDING);

	max[axisA] = a1;
	max[axisB] = b1;

	aabb = AABB(min, max);

	this->k = k;

	this->a0 = a0;
	this->a1 = a1;

	this->b0 = b0;
	this->b1 = b1;

	this->axis = axis;
}

Rectangle::Rectangle(float a0, float a1, float b0, float b1, float k, Vector3D::Axis axis, const Color& color):
	DefaultingMaterialHittable(color)
{
	setBounds(a0, a1, b0, b1, k, axis);
}

Rectangle::Rectangle(float a0, float a1, float b0, float b1, float k, Vector3D::Axis axis, const Material* material):
	DefaultingMaterialHittable(material)
{
	setBounds(a0, a1, b0, b1, k, axis);
}

Vector3D Rectangle::getRandomPoint(const Vector3D& origin) const
{
	Vector3D randomPoint = Vector3D::filledAlongAxis(Utils::randomFloat(a0, a1), Utils::randomFloat(b0, b1), axis, k);

	return randomPoint - origin;
}

bool Rectangle::getBoundingBox(AABB& aabb, float t0, float t1) const
{
	aabb = this->aabb;

	return true;
}

float Rectangle::getPdfValue(const Vector3D& origin, const Vector3D& direction) const
{
	HitResult hitResult;

	if(!hit(hitResult, Ray(origin, direction), 0.001f, Utils::Math::infinity))
		return 0;

	float area = (a1 - a0) * (b1 - b0);
	float distanceSquared = hitResult.t * hitResult.t * direction.lengthSquared();
	float cosine = fabs(direction.dotProduct(hitResult.hitNormal) / direction.length());

	return distanceSquared / (cosine * area);
}

bool Rectangle::hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const
{
	Vector3D origin = ray.getOrigin();
	Vector3D direction = ray.getDirection();

	float t = (k - origin[axis]) / direction[axis];

	if(t < tmin || t > tmax)
		return false;

	float a = origin[axisA] + t * direction[axisA];
	float b = origin[axisB] + t * direction[axisB];

	if(a < a0 || a > a1 || b < b0 || b > b1)
		return false;

	float u = (a - a0) / (a1 - a0);
	float v = (b - b0) / (b1 - b0);

	hitResult = HitResult(ray, ray.at(t), Vector3D::ofStandardBasis(axis), this, material, t, u, v);

	return true;
}
