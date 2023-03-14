#pragma once

#include "../color/Color.h"

class Texture
{
public:
	virtual ~Texture();

	// (u, v) - texture coordinates
	virtual Color getTextureColor(const Vector3D& point, float u, float v) const = 0;
};
