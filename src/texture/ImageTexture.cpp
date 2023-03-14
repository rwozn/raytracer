#include "../Utils.h"
#include "ImageTexture.h"

ImageTexture::ImageTexture(const Image& image):
	image(image)
{

}

Image ImageTexture::getImage() const
{
	return image;
}

void ImageTexture::setImage(const Image& image)
{
	this->image = image;
}

Color ImageTexture::getTextureColor(const Vector3D& point, float u, float v) const
{
	// clamp input texture coordinates to [0,1] x [1,0]
	u = Utils::clamp(u, 0, 1);
	v = 1 - Utils::clamp(v, 0, 1);

	const size_t width = image.getWidth();
	const size_t height = image.getHeight();

	size_t x = u * width;
	size_t y = v * height;

	// clamp integer mapping, since actual coordinates should be less than 1
	if(x >= width)
		x = width - 1;

	if(y >= height)
		y = height - 1;

	return image.getPixel(x, y);
}
