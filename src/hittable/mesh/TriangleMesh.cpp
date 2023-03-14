#include "TriangleMesh.h"
#include "ObjFileLoader.h"

void TriangleMesh::setTriangles(const PolygonMesh& polygonMesh)
{
	triangles = polygonMesh.toTriangles(material);
}

TriangleMesh::TriangleMesh(const std::string& objFilePath, const Color& color):
	DefaultingMaterialHittable(color)
{
	setTriangles(ObjFileLoader::loadPolygonMesh(objFilePath));
}

TriangleMesh::TriangleMesh(const PolygonMesh& polygonMesh, const Color& color):
	DefaultingMaterialHittable(color)
{
	setTriangles(polygonMesh);
}

TriangleMesh::TriangleMesh(const std::string& objFilePath, const Material* material):
	DefaultingMaterialHittable(material)
{
	setTriangles(ObjFileLoader::loadPolygonMesh(objFilePath));
}

TriangleMesh::TriangleMesh(const PolygonMesh& polygonMesh, const Material* material):
	DefaultingMaterialHittable(material)
{
	setTriangles(polygonMesh);
}

TriangleMesh::~TriangleMesh()
{
	for(const Hittable* triangle: triangles.getAll())
		delete triangle;
}

const HittableList& TriangleMesh::getTriangles() const
{
	return triangles;
}

Vector3D TriangleMesh::getRandomPoint(const Vector3D& origin) const
{
	return triangles.getRandomPoint(origin);
}

bool TriangleMesh::getBoundingBox(AABB& aabb, float t0, float t1) const
{
	return triangles.getBoundingBox(aabb, t0, t1);
}

float TriangleMesh::getPdfValue(const Vector3D& origin, const Vector3D& direction) const
{
	return triangles.getPdfValue(origin, direction);
}

bool TriangleMesh::hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const
{
	return triangles.hit(hitResult, ray, tmin, tmax);
}
