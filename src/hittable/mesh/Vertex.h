#pragma once

#include "../../Vector3D.h"

struct Vertex
{
	float u, v;

	Vector3D normal;

	Vector3D position;

	Vertex(const Vector3D& position);
	Vertex(const Vector3D& position, float u, float v);
	Vertex(const Vector3D& position, const Vector3D& normal);
	Vertex(const Vector3D& position, const Vector3D& normal, float u, float v);
};
