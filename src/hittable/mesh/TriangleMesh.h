#pragma once

#include "PolygonMesh.h"
#include "../DefaultingMaterialHittable.h"

class TriangleMesh: public DefaultingMaterialHittable<>
{
	HittableList triangles;

	void setTriangles(const PolygonMesh& polygonMesh);

public:
	TriangleMesh(const std::string& objFilePath, const Color& color);
	TriangleMesh(const PolygonMesh& polygonMesh, const Color& color);
	TriangleMesh(const std::string& objFilePath, const Material* material);
	TriangleMesh(const PolygonMesh& polygonMesh, const Material* material);
	~TriangleMesh();

	const HittableList& getTriangles() const;

	virtual Vector3D getRandomPoint(const Vector3D& origin) const override;
	virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const override;
	virtual float getPdfValue(const Vector3D& origin, const Vector3D& direction) const override;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const override;
};
