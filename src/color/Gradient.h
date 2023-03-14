#pragma once

#include "Color.h"
#include "../Ray.h"

struct Gradient
{
	Color from, to;

	Gradient(const Color& from, const Color& to);

	Color getColor(const Ray& ray) const;
};

