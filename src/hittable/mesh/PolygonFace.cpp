#include "PolygonFace.h"

#include <string>
#include <stdexcept>

PolygonFace::PolygonFace(const std::vector<size_t>& vertexIndices, const std::vector<size_t>& textureCoordinateIndices, const std::vector<size_t>& normalIndices):
	normalIndices(normalIndices),
	vertexIndices(vertexIndices),
	textureCoordinateIndices(textureCoordinateIndices)
{
	if(!textureCoordinateIndices.empty() && textureCoordinateIndices.size() != vertexIndices.size())
		throw std::logic_error("Polygon face has non-empty texture coordinates indices list, but its size (" + std::to_string(textureCoordinateIndices.size()) + ") doesn't match vertex indices list size (" + std::to_string(vertexIndices.size()) + ")");

	if(!normalIndices.empty() && normalIndices.size() != vertexIndices.size())
		throw std::logic_error("Polygon face has non-empty normal indices list, but its size (" + std::to_string(normalIndices.size()) + ") doesn't match vertex indices list size (" + std::to_string(vertexIndices.size()) + ")");
}