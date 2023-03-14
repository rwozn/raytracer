#include "../Utils.h"
#include "PerlinNoise.h"

#include <cmath>

static void permute(int* permutations, size_t n)
{
	for(size_t i = n - 1; i > 0; i--)
		std::swap(permutations[i], permutations[Utils::randomInt(0, i)]);
}

int* PerlinNoise::generatePermutations()
{
	int* permutations = new int[POINTS_COUNT];

	for(size_t i = 0; i < POINTS_COUNT; i++)
		permutations[i] = i;

	permute(permutations, POINTS_COUNT);

	return permutations;
}

PerlinNoise::PerlinNoise()
{
	randomVectors = new Vector3D[POINTS_COUNT];

	for(size_t i = 0; i < POINTS_COUNT; i++)
		randomVectors[i] = Vector3D::random(-1, 1).normalized();

	permutationsX = generatePermutations();
	permutationsY = generatePermutations();
	permutationsZ = generatePermutations();
}

PerlinNoise::~PerlinNoise()
{
	delete[] randomVectors;

	delete[] permutationsX;
	delete[] permutationsY;
	delete[] permutationsZ;
}

static constexpr size_t CUBE_SIZE = 2;

static float perlinInterpolation(Vector3D cube[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE], float u, float v, float w)
{
	float sum = 0;
	
	const float uu = u * u * (3 - u * 2);
	const float vv = v * v * (3 - v * 2);
	const float ww = w * w * (3 - w * 2);
	
	for(size_t i = 0; i < CUBE_SIZE; i++)
		for(size_t j = 0; j < CUBE_SIZE; j++)
			for(size_t k = 0; k < CUBE_SIZE; k++)
			{
				Vector3D weight(u - i, v - j, w - k);

				sum += (i * uu + (1 - i) * (1 - uu)) *
					   (j * vv + (1 - j) * (1 - vv)) *
					   (k * ww + (1 - k) * (1 - ww)) *
					   cube[i][j][k].dotProduct(weight);
			}

	return sum;
}

float PerlinNoise::generateNoise(const Vector3D& point) const
{
	const float fx = floorf(point.x);
	const float fy = floorf(point.y);
	const float fz = floorf(point.z);

	const size_t x = fx;
	const size_t y = fy;
	const size_t z = fz;

	const float u = point.x - fx;
	const float v = point.y - fy;
	const float w = point.z - fz;

	Vector3D cube[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];

	for(size_t i = 0; i < CUBE_SIZE; i++)
		for(size_t j = 0; j < CUBE_SIZE; j++)
			for(size_t k = 0; k < CUBE_SIZE; k++)
				cube[i][j][k] = randomVectors[permutationsX[(x + i) & 255] ^
											  permutationsY[(y + j) & 255] ^
											  permutationsZ[(z + k) & 255]];

	return perlinInterpolation(cube, u, v, w);
}

float PerlinNoise::generateTurbulence(Vector3D point, size_t depth) const
{
	float sum = 0;

	float weight = 1;

	for(size_t i = 0; i < depth; i++)
	{
		sum += weight * generateNoise(point);

		point *= 2;
		weight *= 0.5f;
	}

	return fabs(sum);
}
