#pragma once

#include <vector>

#include "color/Color.h"

class Image
{
	size_t width, height;

	std::vector<std::vector<Color>> pixels;

public:
	Image(const std::string& filePath);
	Image(size_t size, const Color& defaultColor = Color());
	Image(size_t width, size_t height, const Color& defaultColor = Color());

	size_t getSize() const;
	size_t getWidth() const;
	size_t getHeight() const;

	void save(const std::string& filePath) const;

	const Color& getPixel(size_t x, size_t y) const;
	void setPixel(size_t x, size_t y, const Color& color);
};

