#pragma once

#include "Texture.h"
#include "../Image.h"

class ImageTexture: public Texture
{
	Image image;

public:
	ImageTexture(const Image& image);

	Image getImage() const;
	void setImage(const Image& image);

	virtual Color getTextureColor(const Vector3D& point, float u, float v) const override;
};

