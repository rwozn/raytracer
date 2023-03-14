#pragma once

#include "HittableList.h"

class BVHNode: public Hittable
{
	AABB aabb;

	bool createdNodes;

	const Hittable* left;
	const Hittable* right;

public:
	virtual ~BVHNode();

	BVHNode(const HittableList& hittableList, float t0, float t1);
	BVHNode(const HittableList::List& hittables, float t0, float t1);
	BVHNode(HittableList::List hittables, size_t start, size_t end, float t0, float t1);

	virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const override;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const override;
};

