#pragma once

#include "Texture.h"

class SolidColorTexture: public Texture
{
	Color color;

public:
	SolidColorTexture(const Color& color);

	Color getColor() const;
	void setColor(const Color& color);

	virtual Color getTextureColor(const Vector3D& point, float u, float v) const override;
};
