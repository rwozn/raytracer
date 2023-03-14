#include "Vertex.h"

Vertex::Vertex(const Vector3D& position):
	Vertex(Vector3D(), Vector3D(), 0, 0)
{

}

Vertex::Vertex(const Vector3D& position, float u, float v):
	Vertex(position, Vector3D(), u, v)
{

}

Vertex::Vertex(const Vector3D& position, const Vector3D& normal):
	Vertex(position, normal, 0, 0)
{

}

Vertex::Vertex(const Vector3D& position, const Vector3D& normal, float u, float v):
	u(u),
	v(v),
	normal(normal),
	position(position)
{

}