#pragma once

#include "Movable.h"

class Sphere: public Movable
{
	float radius;

public:
	// constructors for static and moving sphere
	Sphere(const Vector3D& origin, float radius, const Color& color);
	Sphere(const Vector3D& origin, float radius, const Material* material);
	Sphere(const Vector3D& startOrigin, const Vector3D& endOrigin, float radius, float startTime, float endTime, const Color& color);
	Sphere(const Vector3D& startOrigin, const Vector3D& endOrigin, float radius, float startTime, float endTime, const Material* material);
	
	float getRadius() const;
	void setRadius(float radius);

	/*
	* point: a given point on the sphere of radius 1, centered at the origin
	* u: returned value [0, 1] of angle around the Y axis from X = -1
	* v: returned value [0, 1] of angle from Y = -1 to Y = +1
	*	<1 0 0> yields <0.50 0.50>		<-1  0  0> yields <0.00 0.50>
	*	<0 1 0> yields <0.50 1.00>		< 0 -1  0> yields <0.50 0.00>
	*	<0 0 1> yields <0.25 0.50>		< 0  0 -1> yields <0.75 0.50>
	*/
	static void getUV(const Vector3D& point, float& u, float& v);

	virtual Vector3D getRandomPoint(const Vector3D& origin) const override;
	virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const override;
	virtual float getPdfValue(const Vector3D& origin, const Vector3D& direction) const override;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const override;
};
