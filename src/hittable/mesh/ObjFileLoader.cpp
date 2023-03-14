#include "ObjFileLoader.h"

#include <fstream>
#include <sstream>

// List of geometric vertices, with (x, y, z, [w]) coordinates,
// w is optional and defaults to 1.0.
// e.g. v "0.123 0.234 0.345 1.0"
Vector3D ObjFileLoader::readVertex(std::stringstream& ss)
{
	float x, y, z;

	ss >> x >> y >> z;

	return Vector3D(x, y, z);
}

// List of vertex normals in (x, y, z) form; normals might not be unit vectors.
Vector3D ObjFileLoader::readVertexNormal(std::stringstream& ss)
{
	float x, y, z;

	ss >> x >> y >> z;

	return Vector3D(x, y, z);
}

// List of texture coordinates, in (u, [v, w]) coordinates,
// these will vary between 0 and 1. v, w are optional and default to 0.
// e.g. "vt 0.500 1 [0]"
void ObjFileLoader::readTextureCoordinates(std::stringstream& ss, float& u, float& v)
{
	v = 0;

	ss >> u >> v;
}

/*
	Faces are defined using lists of vertex, texture and normal indices in the format vertex_index/texture_index/normal_index
	for which each index starts at 1 and increases corresponding to the order in which the referenced element was defined.
	Polygons such as quadrilaterals can be defined by using more than three indices.
*/
PolygonFace ObjFileLoader::readPolygonFace(std::stringstream& ss, size_t verticesCount)
{
	std::vector<size_t> normalIndices;
	std::vector<size_t> vertexIndices;
	std::vector<size_t> textureCoordinateIndices;

	std::string vertexInformation;

	while(ss >> vertexInformation)
	{
		int relativeVertexIndex;
		
		std::stringstream vss(vertexInformation);

		vss >> relativeVertexIndex;

		// vertex index can be positive (starts at 1) or negative (-1 means last element)
		vertexIndices.push_back(relativeVertexIndex > 0 ? relativeVertexIndex - 1 : relativeVertexIndex + verticesCount);

		size_t slashIndex = vertexInformation.find('/');
		
		if(slashIndex == std::string::npos)
			continue;

		size_t textureCoordinatesIndex;
		
		size_t nextSlashIndex = vertexInformation.find('/', slashIndex + 1);
		
		// can be e.g. "12/5" or "2"
		if(nextSlashIndex == std::string::npos)
		{
			vss << vertexInformation.substr(slashIndex + 1);
			
			// if it's e.g. "1/4"
			if(vss >> textureCoordinatesIndex)
				textureCoordinateIndices.push_back(textureCoordinatesIndex);

			continue;
		}

		size_t normalIndex;

		// if it's e.g. "2/5/4" and not "2//4"
		if(nextSlashIndex != slashIndex + 1)
		{
			vss << vertexInformation.substr(slashIndex + 1);

			ss >> textureCoordinatesIndex;
			
			textureCoordinateIndices.push_back(textureCoordinatesIndex);
		}

		vss << vertexInformation.substr(nextSlashIndex + 1);

		vss >> normalIndex;

		normalIndices.push_back(normalIndex);
	}

	return PolygonFace(vertexIndices, textureCoordinateIndices, normalIndices);
}

PolygonMesh ObjFileLoader::loadPolygonMesh(const std::string& filePath)
{
	std::ifstream file(filePath);

	if(file.fail())
		throw std::runtime_error("Unable to open .obj file \"" + filePath + "\"");

	std::vector<float> us;
	std::vector<float> vs;

	std::vector<Vector3D> normals;
	std::vector<Vector3D> vertices;

	std::vector<PolygonFace> faces;

	for(std::string line; getline(file, line);)
	{
		std::string firstToken;

		std::stringstream ss(line);

		ss >> firstToken;

		char letter = firstToken[0];

		if(letter == 'v')
		{
			if(firstToken.length() == 1)
			{
				vertices.push_back(readVertex(ss));

				continue;
			}

			letter = firstToken[1];

			if(letter == 't')
			{
				float u, v;

				readTextureCoordinates(ss, u, v);

				us.push_back(u);
				vs.push_back(v);
			}
			else if(letter == 'n')
				normals.push_back(readVertexNormal(ss));
		}
		else if(letter == 'f')
			faces.push_back(readPolygonFace(ss, vertices.size()));
	}

	return PolygonMesh(us, vs, normals, vertices, faces);
}