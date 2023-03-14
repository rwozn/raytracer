#pragma once

#include "Window.h"
#include "../Image.h"

class DrawingWindow: public Window
{
protected:
	Color backgroundColor;

	void preDraw() const;
	void postDraw() const;

	void clearScreen() const;
	void updateScreen() const;

	void colorPoint(size_t x, size_t y) const;
	void setDrawingColor(const Color& color) const;
	void drawPixel(size_t x, size_t y, const Color& color) const;

	void drawImagePart(const Image& image, size_t startIndex, size_t endIndex) const;
	void drawImagePart(const Image& image, size_t startIndex, size_t endIndex, size_t x, size_t y) const;
	
public:
	DrawingWindow(size_t size, const std::string& title = DEFAULT_TITLE);
	DrawingWindow(size_t width, size_t height, const std::string& title = DEFAULT_TITLE);

	void keepDrawing(const Image& image) const;
	
	void drawImage(const Image& image) const;
	void drawImage(const Image& image, size_t x, size_t y) const;
};
