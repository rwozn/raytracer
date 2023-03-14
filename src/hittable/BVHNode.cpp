#include "BVHNode.h"
#include "../Utils.h"

#include <algorithm>

BVHNode::~BVHNode()
{
	if(!createdNodes)
		return;

	delete left;
	delete right;
}

BVHNode::BVHNode(const HittableList& hittableList, float t0, float t1):
	BVHNode(hittableList.getAll(), t0, t1)
{

}

BVHNode::BVHNode(const HittableList::List& hittables, float t0, float t1):
	BVHNode(hittables, 0, hittables.size(), t0, t1)
{

}

static bool compareAABBs(const Hittable* a, const Hittable* b, Vector3D::Axis axis)
{
	AABB aabbA, aabbB;

	if(!a->getBoundingBox(aabbA, 0, 0) || !b->getBoundingBox(aabbB, 0, 0))
		throw std::logic_error("No bounding box in BVHNode::compareAABBs");

	return aabbA.getMin()[axis] < aabbB.getMin()[axis];
}

static bool compareAABBsX(const Hittable* a, const Hittable* b)
{
	return compareAABBs(a, b, Vector3D::X);
}

static bool compareAABBsY(const Hittable* a, const Hittable* b)
{
	return compareAABBs(a, b, Vector3D::Y);
}

static bool compareAABBsZ(const Hittable* a, const Hittable* b)
{
	return compareAABBs(a, b, Vector3D::Z);
}

static auto getAABBComparator(Vector3D::Axis axis)
{
	switch(axis)
	{
		case Vector3D::X: return compareAABBsX;
		case Vector3D::Y: return compareAABBsY;
	}

	return compareAABBsZ;
}

BVHNode::BVHNode(HittableList::List hittables, size_t start, size_t end, float t0, float t1):
	createdNodes(false)
{
	Vector3D::Axis axis = Utils::randomInt(Vector3D::X, Vector3D::Z);

	auto comparator = getAABBComparator(axis);

	const size_t span = end - start;

	if(span == 1)
		left = right = hittables[start];
	else if(span == 2)
	{
		const Hittable* a = hittables[start];
		const Hittable* b = hittables[start + 1];

		if(comparator(a, b))
		{
			left = a;
			right = b;
		}
		else
		{
			left = b;
			right = a;
		}
	}
	else
	{
		std::sort(hittables.begin() + start, hittables.begin() + end, comparator);

		createdNodes = true;

		size_t mid = start + span / 2;

		left = new BVHNode(hittables, start, mid, t0, t1);
		right = new BVHNode(hittables, mid, end, t0, t1);
	}

	AABB leftAABB, rightAABB;

	// can happen when sending something that doesn't have a bounding box,
	// e.g. an infinite plane
	if(!left->getBoundingBox(leftAABB, t0, t1) || !right->getBoundingBox(rightAABB, t0, t1))
		throw std::logic_error("No bounding box in BVHNode constructor");

	aabb = leftAABB.getSurroundingBox(rightAABB);
}

bool BVHNode::getBoundingBox(AABB& aabb, float t0, float t1) const
{
	aabb = this->aabb;

	return true;
}

bool BVHNode::hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const
{
	if(!aabb.hit(ray, tmin, tmax))
		return false;

	bool hitLeft = left->hit(hitResult, ray, tmin, tmax);
	bool hitRight = right->hit(hitResult, ray, tmin, hitLeft ? hitResult.t : tmax);

	return hitLeft || hitRight;
}
