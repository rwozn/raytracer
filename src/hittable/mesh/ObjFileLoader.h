#pragma once

#include "PolygonMesh.h"

class ObjFileLoader
{
	static Vector3D readVertex(std::stringstream& ss);
	static Vector3D readVertexNormal(std::stringstream& ss);
	static void readTextureCoordinates(std::stringstream& ss, float& u, float& v);
	static PolygonFace readPolygonFace(std::stringstream& ss, size_t verticesCount);
	
public:
	static PolygonMesh loadPolygonMesh(const std::string& filePath);
};
