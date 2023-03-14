#include "Image.h"

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

static const size_t BYTES_PER_PIXEL = 3;

Image::Image(const std::string& filePath)
{
	int x, y;

	int componentsPerPixel = BYTES_PER_PIXEL;

	stbi_uc* rawImage = stbi_load(filePath.c_str(), &x, &y, &componentsPerPixel, BYTES_PER_PIXEL);

	if(!rawImage)
		throw std::runtime_error("Unable to load image file \"" + filePath + "\": " + stbi_failure_reason());

	width = x;
	height = y;

	pixels.resize(width);

	const size_t BYTES_PER_SCANLINE = width * BYTES_PER_PIXEL;

	for(size_t i = 0; i < width; i++)
	{
		pixels[i].resize(height);

		for(size_t j = 0; j < height; j++)
		{
			stbi_uc* rgb = rawImage + i * BYTES_PER_PIXEL + j * BYTES_PER_SCANLINE;

			float r = rgb[0] / 255.f;
			float g = rgb[1] / 255.f;
			float b = rgb[2] / 255.f;

			pixels[i][j] = Color(r, g, b);
		}
	}

	stbi_image_free(rawImage);
}

Image::Image(size_t size, const Color& defaultColor):
	Image(size, size, defaultColor)
{

}

Image::Image(size_t width, size_t height, const Color& defaultColor):
	width(width),
	height(height)
{
	pixels.resize(width);

	for(size_t i = 0; i < width; i++)
	{
		pixels[i].resize(height);

		for(size_t j = 0; j < height; j++)
			pixels[i][j] = defaultColor;
	}
}

size_t Image::getSize() const
{
	return width * height;
}

size_t Image::getWidth() const
{
	return width;
}

size_t Image::getHeight() const
{
	return height;
}

void Image::save(const std::string& filePath) const
{
	const size_t BYTES_PER_SCANLINE = width * BYTES_PER_PIXEL;

	stbi_uc* rawImage = new stbi_uc[BYTES_PER_SCANLINE * height];

	for(size_t i = 0; i < width; i++)
		for(size_t j = 0; j < height; j++)
		{
			const Color& pixel = pixels[i][j];

			stbi_uc* rgb = rawImage + i * BYTES_PER_PIXEL + j * BYTES_PER_SCANLINE;

			rgb[0] = pixel.r * 255;
			rgb[1] = pixel.g * 255;
			rgb[2] = pixel.b * 255;
		}

	bool success = stbi_write_png(filePath.c_str(), width, height, BYTES_PER_PIXEL, rawImage, 0);

	delete[] rawImage;

	if(!success)
		throw std::runtime_error("Unable to save image as \"" + filePath + "\"");
}

const Color& Image::getPixel(size_t x, size_t y) const
{
	return pixels.at(x).at(y);
}

void Image::setPixel(size_t x, size_t y, const Color& color)
{
	pixels.at(x).at(y) = color;
}
