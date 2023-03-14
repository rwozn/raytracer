#pragma once

#include <string>
#include <SDL2/SDL.h>

class Window
{
protected:
	SDL_Window* window;
	SDL_Renderer* renderer;
	
	const size_t width, height;
	
	static const std::string DEFAULT_TITLE;

	using SDLCallFailPredicate = bool (*)(int returnValue);

	static const SDLCallFailPredicate failPredicateOnMinusOne;
	static const SDLCallFailPredicate failPredicateOnNegative;

	// return true when quit event has been received
	bool pollEvent() const;
	bool pollEvents() const;

	void delay(size_t ms) const;

	static std::string getLastSDLErrorMessage();

	static void ensureSDLCallSuccess(int returnValue, SDLCallFailPredicate failPredicate);

public:
	Window(size_t size, const std::string& title = DEFAULT_TITLE);
	Window(size_t width, size_t height, const std::string& title = DEFAULT_TITLE);
	~Window();

	void keepOpen() const;

	size_t getWidth() const;
	size_t getHeight() const;

	void setTitle(const std::string& title);
};

