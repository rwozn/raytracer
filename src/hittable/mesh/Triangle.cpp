#include "Triangle.h"
#include "../../Utils.h"

#include <cmath>
#include <string>

void Triangle::validateVertexId(VertexId vertexId)
{
	if(vertexId >= VERTICES_COUNT)
		throw std::out_of_range("Invalid vertex id: " + std::to_string(vertexId) + "; must be " + std::to_string(A) + ", " + std::to_string(B) + " or " + std::to_string(C));
}

void Triangle::updateBounds()
{
	Vector3D min = a.position.min(b.position).min(c.position);
	Vector3D max = a.position.max(b.position).max(c.position);

	// the bounding must have non-zero width in each dimension,
	// so pad the axes along which the min and max positions
	// overlap
	static constexpr float PADDING = 1e-4f;

	for(Vector3D::Axis i = Vector3D::X; i <= Vector3D::Z; i++)
	{
		float& a = min[i];
		float& b = max[i];

		if(a == b)
		{
			a -= PADDING;
			b += PADDING;
		}
	}

	aabb = AABB(min, max);
}

Triangle::Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Color& color):
	DefaultingMaterialHittable(color),
	a(a),
	b(b),
	c(c)
{
	updateBounds();
}

Triangle::Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Material* material):
	DefaultingMaterialHittable(material),
	a(a),
	b(b),
	c(c)
{
	updateBounds();
}

const Vertex& Triangle::operator[](VertexId vertexId) const
{
	validateVertexId(vertexId);

	switch(vertexId)
	{
		case A: return a;
		case B: return b;
	}

	return c;
}

bool Triangle::getBoundingBox(AABB& aabb, float t0, float t1) const
{
	aabb = this->aabb;

	return true;
}

// Möller-Trumbore algorithm
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-polygon-mesh/ray-tracing-polygon-mesh-part-2
bool Triangle::hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const
{
	Vector3D direction = ray.getDirection();

	Vector3D edgeB = c.position - a.position;

	Vector3D p = direction.crossProduct(edgeB);

	Vector3D edgeA = b.position - a.position;

	float determinant = edgeA.dotProduct(p);

	if(fabs(determinant) < Utils::epsilon)
		return false; // they are parallel so they don't intersect

	float inverseDeterminant = 1 / determinant;

	Vector3D s = ray.getOrigin() - a.position;

	float u = s.dotProduct(p) * inverseDeterminant;

	if(u < 0 || u > 1)
		return false;

	Vector3D q = s.crossProduct(edgeA);

	float v = direction.dotProduct(q) * inverseDeterminant;

	if(v < 0 || u + v > 1)
		return false;

	float t = edgeB.dotProduct(q) * inverseDeterminant;

	if(t <= Utils::epsilon || t < tmin || t > tmax)
		return false;

	Vector3D uv(1 - u - v, u, v);

	u = uv.x * a.u + uv.y * b.u + uv.z * c.u;
	v = uv.x * a.v + uv.y * b.v + uv.z * c.v;

	// smooth shading using vertex normals
	Vector3D hitNormal = a.normal * uv.x + b.normal * uv.y + c.normal * uv.z;
	
	hitResult = HitResult(ray, ray.at(t), hitNormal, this, material, t, u, v);

	return true;
}
