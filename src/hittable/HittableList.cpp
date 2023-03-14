#include "../Utils.h"
#include "Hittablelist.h"

void HittableList::clear()
{
    hittables.clear();
}

bool HittableList::isEmpty() const
{
    return hittables.empty();
}

size_t HittableList::getCount() const
{
    return hittables.size();
}

const HittableList::List& HittableList::getAll() const
{
    return hittables;
}

void HittableList::add(const Hittable* hittable)
{
    hittables.push_back(hittable);
}

const Hittable* HittableList::get(size_t index) const
{
    return hittables.at(index);
}

Vector3D HittableList::getRandomPoint(const Vector3D& origin) const
{
    return hittables[Utils::randomInt(0, hittables.size() - 1)]->getRandomPoint(origin);
}

bool HittableList::getBoundingBox(AABB& aabb, float t0, float t1) const
{
    if(hittables.empty())
        return false;

    AABB temp;

    bool foundAny = false;

    for(const Hittable* hittable: hittables)
    {
        if(!hittable->getBoundingBox(temp, t0, t1))
            return false;

        aabb = foundAny ? aabb.getSurroundingBox(temp) : temp;

        foundAny = true;
    }

    return true;
}

float HittableList::getPdfValue(const Vector3D& origin, const Vector3D& direction) const
{
    if(hittables.empty())
        return 0;

    float sum = 0;

    const float weight = 1.f / hittables.size();

    for(const Hittable* hittable: hittables)
        sum += weight * hittable->getPdfValue(origin, direction);

    return sum;
}

bool HittableList::hit(HitResult& hitResult, const Ray& ray, float tmin, float tmax) const
{
    if(hittables.empty())
        return false;

    HitResult temp;

    bool hitAny = false;

    float closest = tmax;

    for(const Hittable* hittable: hittables)
        if(hittable->hit(temp, ray, tmin, closest))
        {
            hitAny = true;

            closest = temp.t;

            hitResult = temp;
        }

    return hitAny;
}
