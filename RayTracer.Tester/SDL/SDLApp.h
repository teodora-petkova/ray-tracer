#pragma once

#pragma warning(push, 0)
#include "SDL.h"
#include <string>
#include <exception>
#pragma warning(pop)

class SDLApp {
public:
	// title - window's title
	// x, y coordinates on the screen, in pixels, of the window's upper left corner
	// w, h = window's length and height in pixels)
	SDLApp(const std::string& title, int x, int y, int w, int h, Uint32 flags = SDL_WINDOW_OPENGL);
	virtual ~SDLApp();

	// no copying or moving of the class objects
	SDLApp(const SDLApp& other) = delete;
	SDLApp& operator=(const SDLApp& other) = delete;
	SDLApp(SDLApp&& other) = delete;
	SDLApp& operator=(SDLApp&& other) = delete;

	void Update(unsigned char* pixels, size_t size);

private:
	// Managed SDL2_Window object
	SDL_Window* window;
	// use SDL_CreateRenderer in order for draw calls to affect this window.
	SDL_Renderer* renderer;
};