#include "DrawingWindow.h"

void DrawingWindow::preDraw() const
{
	clearScreen();
}

void DrawingWindow::postDraw() const
{
	updateScreen();
}

void DrawingWindow::clearScreen() const
{
	setDrawingColor(backgroundColor);

	ensureSDLCallSuccess(SDL_RenderClear(renderer), failPredicateOnNegative);
}

void DrawingWindow::updateScreen() const
{
	SDL_RenderPresent(renderer);
}

void DrawingWindow::colorPoint(size_t x, size_t y) const
{
	ensureSDLCallSuccess(SDL_RenderDrawPoint(renderer, x, y), failPredicateOnNegative);
}

void DrawingWindow::setDrawingColor(const Color& color) const
{
	color.validateChannels();

	ensureSDLCallSuccess(SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, SDL_ALPHA_OPAQUE), failPredicateOnNegative);
}

void DrawingWindow::drawPixel(size_t x, size_t y, const Color& color) const
{
	setDrawingColor(color);
	colorPoint(x, y);
}

void DrawingWindow::drawImagePart(const Image& image, size_t startIndex, size_t endIndex) const
{
	drawImagePart(image, startIndex, endIndex, 0, 0);
}

void DrawingWindow::drawImagePart(const Image& image, size_t startIndex, size_t endIndex, size_t x, size_t y) const
{
	preDraw();

	const size_t offsetX = x;
	const size_t offsetY = y;
	
	const size_t height = image.getHeight();
	
	for(size_t i = startIndex; i <= endIndex; i++)
	{
		x = i / height;
		y = i % height;

		drawPixel(x + offsetX, y + offsetY, image.getPixel(x, y));
	}

	postDraw();
}

DrawingWindow::DrawingWindow(size_t size, const std::string& title):
	DrawingWindow(size, size, title)
{

}

DrawingWindow::DrawingWindow(size_t width, size_t height, const std::string& title):
	Window(width, height, title),
	backgroundColor(Color::WHITE)
{
	clearScreen();
}

void DrawingWindow::keepDrawing(const Image& image) const
{
	while(!pollEvent())
		drawImage(image);
}

void DrawingWindow::drawImage(const Image& image) const
{
	drawImage(image, 0, 0);
}

void DrawingWindow::drawImage(const Image& image, size_t x, size_t y) const
{
	preDraw();

	const size_t width = image.getWidth();
	const size_t height = image.getHeight();

	for(size_t i = 0; i < width; i++)
		for(size_t j = 0; j < height; j++)
			drawPixel(x + i, y + j, image.getPixel(i, j));

	postDraw();
}
