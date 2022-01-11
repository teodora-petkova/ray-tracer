#include "SDLApp.h"

SDLApp::SDLApp(const std::string& title, int x, int y, int w, int h,
	Uint32 flags)
{
	if (SDL_Init(flags) != 0)
	{
		std::string sdlError = SDL_GetError();
		const char* message = ("SDL_Init error: " + sdlError).c_str();
		throw std::exception(message);
	}

	if ((this->window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags)) == nullptr)
	{
		std::string sdlError = SDL_GetError();
		const char* message = ("SDL_CreateWindow error: " + sdlError).c_str();
		throw std::exception(message);
	}

	this->renderer = SDL_CreateRenderer(this->window, -1, 0);
}

void SDLApp::Update(unsigned char* pixels, size_t size,
	const char* fileToSaveBMP)
{
	SDL_Surface* surface = SDL_GetWindowSurface(this->window);
	SDL_memcpy(surface->pixels, pixels, size);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderer, surface);
	SDL_UpdateWindowSurface(this->window);

	//clear screen
	SDL_RenderClear(this->renderer);

	// render texture to screen
	SDL_RenderCopy(this->renderer, texture, NULL, NULL);

	// up until now everything was drawn behind the scenes
	// now show the new content of the window
	SDL_RenderPresent(this->renderer);

	if (fileToSaveBMP != "")
	{
		SDL_SaveBMP(surface, fileToSaveBMP);
	}

	// free resources
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

	surface = NULL;
	texture = NULL;
}

SDLApp::~SDLApp() {
	// in the reverse order from the constructor

	if (this->renderer != nullptr)
		SDL_DestroyRenderer(this->renderer);
	this->renderer = NULL;

	if (this->window != nullptr)
		SDL_DestroyWindow(this->window);
	this->window = NULL;

	SDL_Quit();
}