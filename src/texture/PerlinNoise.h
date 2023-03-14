#pragma once

#include "../Vector3D.h"

class PerlinNoise
{
	int* permutationsX;
	int* permutationsY;
	int* permutationsZ;

	Vector3D* randomVectors;

	static int* generatePermutations();

	static constexpr size_t POINTS_COUNT = 256;

public:
	PerlinNoise();
	~PerlinNoise();

	float generateNoise(const Vector3D& point) const;
	float generateTurbulence(Vector3D point, size_t depth = 7) const;
};

