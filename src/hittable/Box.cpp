#include "Box.h"

void Box::addSides(Vector3D::Axis axis)
{
	Vector3D::Axis axisA, axisB;

	Vector3D::getRemainingAxes(axis, axisA, axisB);

	Vector3D min = aabb.getMin();
	Vector3D max = aabb.getMax();
	
	const float a0 = min[axisA];
	const float a1 = max[axisA];

	const float b0 = min[axisB];
	const float b1 = max[axisB];

	static const size_t sidesCount = 2;

	const float ks[sidesCount] = {min[axis], max[axis]};

	for(size_t i = 0; i < sidesCount; i++)
		sides.add(new Rectangle(a0, a1, b0, b1, ks[i], axis, material));
}

void Box::createBounds(const Vector3D& min, const Vector3D& max)
{
	aabb = AABB(min, max);

	for(Vector3D::Axis i = Vector3D::X; i <= Vector3D::Z; i++)
		addSides(i);
}

Box::Box(const Vector3D& min, const Vector3D& max, const Color& color):
	DefaultingMaterialHittable(color)
{
	createBounds(min, max);
}

Box::Box(const Vector3D& min, const Vector3D& max, const Material* material):
	DefaultingMaterialHittable(material)
{
	createBounds(min, max);
}

Box::~Box()
{
	for(const Hittable* side: sides.getAll())
		delete side;
}

bool Box::getBoundingBox(AABB& aabb, float t0, float t1) const
{
	aabb = this->aabb;

	return true;
}

bool Box::hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const
{
	return sides.hit(hitResult, ray, tmin, tmax);
}
