#pragma once

#include <vector>

struct PolygonFace
{
	std::vector<size_t> normalIndices;
	std::vector<size_t> vertexIndices;
	std::vector<size_t> textureCoordinateIndices;

	PolygonFace(const std::vector<size_t>& vertexIndices, const std::vector<size_t>& textureCoordinateIndices, const std::vector<size_t>& normalIndices);
};
