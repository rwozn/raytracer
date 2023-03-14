#pragma once

#include "Vertex.h"
#include "../DefaultingMaterialHittable.h"

class Triangle: public DefaultingMaterialHittable<>
{
public:
	using VertexId = Vector3D::Axis;

	static constexpr VertexId A = Vector3D::X;
	static constexpr VertexId B = Vector3D::Y;
	static constexpr VertexId C = Vector3D::Z;

	static constexpr size_t VERTICES_COUNT = Vector3D::AXES_COUNT;
	
private:
	AABB aabb;
	
	Vertex a, b, c;

	static void validateVertexId(VertexId vertexId);

	void updateBounds();

public:
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Color& color);
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Material* material);

	const Vertex& operator[](VertexId vertexId) const;

	virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const override;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const override;
};
