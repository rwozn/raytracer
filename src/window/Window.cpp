#include "Window.h"

#include <stdexcept>

const std::string Window::DEFAULT_TITLE = "Window";

const Window::SDLCallFailPredicate Window::failPredicateOnNegative = [](int returnValue){return returnValue < 0;};
const Window::SDLCallFailPredicate Window::failPredicateOnMinusOne = [](int returnValue){return returnValue == -1;};

bool Window::pollEvent() const
{
	SDL_Event event;

	return SDL_PollEvent(&event) && event.type == SDL_QUIT;
}

bool Window::pollEvents() const
{
	SDL_Event event;

	bool receivedQuitEvent = false;

	while(SDL_PollEvent(&event))
		if(event.type == SDL_QUIT)
			receivedQuitEvent = true;

	return receivedQuitEvent;
}

void Window::delay(size_t ms) const
{
	SDL_Delay(ms);
}

std::string Window::getLastSDLErrorMessage()
{
	return SDL_GetError();
}

void Window::ensureSDLCallSuccess(int returnValue, SDLCallFailPredicate failPredicate)
{
	if(failPredicate(returnValue))
		throw std::runtime_error("SDL call failed: " + getLastSDLErrorMessage());
}

Window::Window(size_t size, const std::string& title):
	Window(size, size, title)
{

}

Window::Window(size_t width, size_t height, const std::string& title):
	window(nullptr),
	renderer(nullptr),
	width(width),
	height(height)
{
	ensureSDLCallSuccess(SDL_Init(SDL_INIT_VIDEO), failPredicateOnNegative);
	ensureSDLCallSuccess(SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer), failPredicateOnMinusOne);

	setTitle(title);

	pollEvents();
}

Window::~Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void Window::keepOpen() const
{
	while(!pollEvents())
		delay(1);
}

size_t Window::getWidth() const
{
	return width;
}

size_t Window::getHeight() const
{
	return height;
}

void Window::setTitle(const std::string& title)
{
	SDL_SetWindowTitle(window, title.c_str());
}