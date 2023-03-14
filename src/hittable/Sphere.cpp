#include "Sphere.h"
#include "../Utils.h"
#include "../OrthonormalBasis.h"

#include <cmath>

Sphere::Sphere(const Vector3D& origin, float radius, const Color& color):
	Sphere(origin, origin, radius, 0, 0, color)
{

}

Sphere::Sphere(const Vector3D& origin, float radius, const Material* material):
	Sphere(origin, origin, radius, 0, 0, material)
{

}

Sphere::Sphere(const Vector3D& startOrigin, const Vector3D& endOrigin, float radius, float startTime, float endTime, const Color& color):
	Movable(startOrigin, endOrigin, startTime, endTime, color),
	radius(radius)
{

}

Sphere::Sphere(const Vector3D& startOrigin, const Vector3D& endOrigin, float radius, float startTime, float endTime, const Material* material):
	Movable(startOrigin, endOrigin, startTime, endTime, material),
	radius(radius)
{

}

float Sphere::getRadius() const
{
	return radius;
}

void Sphere::setRadius(float radius)
{
	this->radius = radius;
}

void Sphere::getUV(const Vector3D& point, float& u, float& v)
{
	float theta = acos(-point.y);
	float phi = atan2(-point.z, point.x) + Utils::Math::pi;

	u = phi / (Utils::Math::pi * 2);
	v = theta / Utils::Math::pi;
}

Vector3D Sphere::getRandomPoint(const Vector3D& origin) const
{
	Vector3D direction = this->origin - origin;

	float distanceSquared = direction.lengthSquared();

	OrthonormalBasis uvw(direction);

	return uvw.local(Vector3D::randomToSphere(radius, distanceSquared));
}

bool Sphere::getBoundingBox(AABB& aabb, float t0, float t1) const
{
	if(isMoving())
	{
		Vector3D origin = getOrigin(t0);

		AABB a(origin - radius, origin + radius);

		origin = getOrigin(t1);

		AABB b(origin - radius, origin + radius);

		aabb = a.getSurroundingBox(b);
	}
	else
		aabb = AABB(origin - radius, origin + radius);

	return true;
}

float Sphere::getPdfValue(const Vector3D& origin, const Vector3D& direction) const
{
	HitResult hitResult;

	if(!hit(hitResult, Ray(origin, direction), 0.001f, Utils::Math::infinity))
		return 0;

	float cosThetaMax = sqrt(1 - radius * radius / (this->origin - origin).lengthSquared());

	float solidAngle = 2 * Utils::Math::pi * (1 - cosThetaMax);

	return 1 / solidAngle;
}

bool Sphere::hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const
{
	Vector3D direction = ray.getDirection();

	Vector3D movedOrigin = getOrigin(ray.getTime());

	Vector3D offset = ray.getOrigin() - movedOrigin;

	float a = direction.lengthSquared();
	float halfb = offset.dotProduct(direction);
	float c = offset.lengthSquared() - radius * radius;

	float discriminant = halfb * halfb - a * c;

	if(discriminant < 0)
		return false;

	float sqrtd = sqrt(discriminant);

	float root = (-halfb - sqrtd) / a;

	// Find the nearest root that lies in the acceptable range
	if(root < tmin || root > tmax)
	{
		root = (-halfb + sqrtd) / a;

		if(root < tmin || root > tmax)
			return false;
	}

	float t = root;

	Vector3D hitPoint = ray.at(t);

	Vector3D outwardNormal = (hitPoint - movedOrigin) / radius;

	float u, v;

	getUV(outwardNormal, u, v);

	hitResult = HitResult(ray, hitPoint, outwardNormal, this, material, t, u, v);

	return true;
}
