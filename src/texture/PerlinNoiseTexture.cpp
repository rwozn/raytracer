#include "PerlinNoiseTexture.h"

#include <cmath>

PerlinNoiseTexture::PerlinNoiseTexture(float scale):
	scale(scale)
{

}

float PerlinNoiseTexture::getScale() const
{
	return scale;
}

void PerlinNoiseTexture::setScale(float scale)
{
	this->scale = scale;
}

Color PerlinNoiseTexture::getTextureColor(const Vector3D& point, float u, float v) const
{
	return Color::WHITE * 0.5f * (sin(point.z * scale + perlinNoise.generateTurbulence(point) * 10) + 1);
}
