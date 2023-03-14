#pragma once

#include "Hittable.h"

#include <vector>

class HittableList: public Hittable
{
public:
    using List = std::vector<const Hittable*>;

protected:
	List hittables;

public:
	void clear();
	
	bool isEmpty() const;

	size_t getCount() const;

	const List& getAll() const;

	void add(const Hittable* hittable);

    const Hittable* get(size_t index) const;
    
	virtual Vector3D getRandomPoint(const Vector3D& origin) const override;
    virtual bool getBoundingBox(AABB& aabb, float t0, float t1) const override;
	virtual float getPdfValue(const Vector3D& origin, const Vector3D& direction) const override;
	virtual bool hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const override;
};
