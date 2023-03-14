#pragma once

#include "Texture.h"
#include "PerlinNoise.h"

class PerlinNoiseTexture: public Texture
{
	float scale;

	PerlinNoise perlinNoise;

public:
	PerlinNoiseTexture(float scale = 1);

	float getScale() const;
	void setScale(float scale);

	virtual Color getTextureColor(const Vector3D& point, float u, float v) const override;
};

