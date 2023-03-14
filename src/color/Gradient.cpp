#include "Gradient.h"

Gradient::Gradient(const Color& from, const Color& to):
	from(from),
	to(to)
{

}

Color Gradient::getColor(const Ray& ray) const
{
	float t = (ray.getDirection().normalized().y + 1) / 2;

	return from * t + to * (1 - t);
}