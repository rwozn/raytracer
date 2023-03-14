#include "PolygonMesh.h"

#include <string>

// https://stackoverflow.com/questions/64314361/triangle-normal-vs-vertex-normal
void PolygonMesh::calculateVertexNormals()
{
	// remove any normals read from the .obj file
	normals.clear();
	normals.resize(vertices.size());

	std::vector<size_t> counters(vertices.size());
	
	for(PolygonFace& face: faces)
	{
		std::vector<size_t>& normalIndices = face.normalIndices;
		
		const std::vector<size_t>& vertexIndices = face.vertexIndices;

		normalIndices.clear();
		normalIndices.resize(vertexIndices.size());

		for(size_t i = 0; i < vertexIndices.size() - 2; i++)
		{
			size_t ida = vertexIndices[i];
			size_t idb = vertexIndices[i + 1];
			size_t idc = vertexIndices[i + 2];

			Vector3D a = vertices[ida];
			Vector3D b = vertices[idb];
			Vector3D c = vertices[idc];

			Vector3D normal = (b - a).crossProduct(c - a).normalized();

			for(size_t j = 0; j < 3; j++)
			{
				size_t k = i + j;
				size_t l = vertexIndices[k];

				counters[l]++;
				normals[l] += normal;
				normalIndices[k] = l;
			}
		}
	}

	for(size_t i = 0; i < normals.size(); i++)
		if(counters[i] > 0)
			normals[i] /= counters[i];
		else
			normals[i] = normals[i].normalized();
}

Vertex PolygonMesh::getFaceVertex(const PolygonFace& face, size_t faceVertexIndex) const
{	
	float u = 0;
	float v = 0;

	if(!face.textureCoordinateIndices.empty())
	{
		size_t textureCoordinateIndex = face.textureCoordinateIndices[faceVertexIndex];

		u = us[textureCoordinateIndex];
		v = vs[textureCoordinateIndex];
	}
	else if(faceVertexIndex == Triangle::B) // set those so that the formula in Triangle::hit works if `uv` aren't given
		u = 1;
	else if(faceVertexIndex == Triangle::C)
		v = 0;

	Vector3D normal = normals[face.normalIndices[faceVertexIndex]];

	Vector3D position = vertices[face.vertexIndices[faceVertexIndex]];

	return Vertex(position, normal, u, v);
}

Triangle* PolygonMesh::createTriangle(const PolygonFace& face, size_t triangleIndex, const Material* material) const
{
	// if there's only one triangle there's no need to affect its creation
	bool isLastTriangle = face.vertexIndices.size() > 3 && triangleIndex == face.vertexIndices.size() - 3;
	
	// if the face contains more than 1 triangle make the last one consisting of
	// the last 2 vertices connected to the first one so that the last vertices
	// are connected to the first one
	size_t i = isLastTriangle ? triangleIndex + 1 : triangleIndex;
	size_t j = i + 1;
	size_t k = isLastTriangle ? 0 : j + 1;

	Vertex a = getFaceVertex(face, i);
	Vertex b = getFaceVertex(face, j);
	Vertex c = getFaceVertex(face, k);

	return new Triangle(a, b, c, material);
}

PolygonMesh::PolygonMesh(const std::vector<float>& us, const std::vector<float>& vs, const std::vector<Vector3D>& normals, const std::vector<Vector3D>& vertices, const std::vector<PolygonFace>& faces):
	us(us),
	vs(vs),
	normals(normals),
	vertices(vertices),
	faces(faces)
{
	for(const PolygonFace& face: faces)
		if(face.vertexIndices.size() < 3)
			throw std::logic_error("Polygon mesh face has " + std::to_string(face.vertexIndices.size()) + " vertices but should have at least 3");

	calculateVertexNormals();
}

HittableList PolygonMesh::toTriangles(const Material* material) const
{
	HittableList triangles;

	for(const PolygonFace& face: faces)
	{
		const std::vector<size_t>& vertexIndices = face.vertexIndices;

		for(size_t i = 0; i < vertexIndices.size() - 2; i++)
			triangles.add(createTriangle(face, i, material));
	}

	return triangles;
}
