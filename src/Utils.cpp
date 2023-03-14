#include "Utils.h"

#include <random>

float Utils::Math::deg2rad(float deg)
{
	return deg * pi / 180;
}

float Utils::Math::rad2deg(float rad)
{
	return rad * 180 / pi;
}

float Utils::randomFloat()
{
	return randomFloat(0, 1);
}

static std::mt19937 RNG((std::random_device())());

int Utils::randomInt(int min, int max)
{
	return std::uniform_int_distribution<>(min, max)(RNG);
}

float Utils::randomFloat(float min, float max)
{
	return std::uniform_real_distribution<float>(min, max)(RNG);
}

float Utils::clamp(float value, float min, float max)
{
	if(value < min)
		return min;

	return value > max ? max : value;
}
