#pragma once

#include "Vertex.h"
#include "Triangle.h"
#include "PolygonFace.h"
#include "../HittableList.h"

class PolygonMesh
{
	// uv texture coordinates
	std::vector<float> us;
	std::vector<float> vs;

	std::vector<Vector3D> normals;
	std::vector<Vector3D> vertices;

	std::vector<PolygonFace> faces;

	void calculateVertexNormals();
	
	Vertex getFaceVertex(const PolygonFace& face, size_t faceVertexIndex) const;
	
	Triangle* createTriangle(const PolygonFace& face, size_t firstFaceVertexIndex, const Material* material) const;

public:
	PolygonMesh(const std::vector<float>& us, const std::vector<float>& vs, const std::vector<Vector3D>& normals, const std::vector<Vector3D>& vertices, const std::vector<PolygonFace>& faces);

	HittableList toTriangles(const Material* material) const;
};
