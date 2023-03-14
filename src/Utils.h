#pragma once

#include <limits>

namespace Utils
{
	namespace Math
	{
        constexpr float pi = 3.1415926535f;
        constexpr float infinity = std::numeric_limits<float>::infinity();

		float deg2rad(float deg);
		float rad2deg(float rad);
	}

	constexpr float epsilon = std::numeric_limits<float>::epsilon();
	
	// random float in [0, 1)
	float randomFloat();

	// random int in [min, max]
	int randomInt(int min, int max);

	// random float in [min, max)
	float randomFloat(float min, float max);

	float clamp(float value, float min, float max);
}
