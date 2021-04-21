#pragma warning(push, 0)
#include <iostream>
#include <ctime>
#include "sdl.h"
#pragma warning(pop)

#include "source\raytracer.h"
#include "source\scene.h"
#include "Source\readfile.h"

using namespace std;

SDL_Texture* updateTexture(const char* sceneFile,
	SDL_Window& window,
	SDL_Renderer& renderer,
	int size,
	int x, int y)
{
	SDL_Surface* surface = SDL_GetWindowSurface(&window);

	clock_t begin = clock();

	RayTracer r = RayTracer();

	Scene scene = ReadScene::readSceneFile(sceneFile);
	scene.Camera.updateLookAt(x, y);

	unsigned char* pixels = r.execute(scene);

	clock_t end = clock();

	float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
	cout << elapsed_secs << '\n';

	SDL_memcpy(surface->pixels, pixels, size);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(&renderer, surface);
	SDL_UpdateWindowSurface(&window);
	SDL_FreeSurface(surface);
	return texture;
}

//------------------------------------------------------------
//  Main Function
//------------------------------------------------------------
int main(int argc, char* argv[])
{
	const char* sceneFile = argv[1];

	bool quit = false;
	SDL_Event event;
	int x = 0;
	int y = 0;
	int rows = 640;
	int columns = 480;
	int size = rows * columns * sizeof(unsigned char) * 4;

	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		printf("SDL Init error: %s", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Simple Ray tracer", // window's title
		10, 25, // coordinates on the screen, in pixels, of the window's upper left corner
		rows, columns, // window's length and height in pixels
		SDL_WINDOW_OPENGL);

	// We must call SDL_CreateRenderer in order for draw calls to affect this window.
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture* texture = updateTexture(sceneFile, *window, *renderer, size, 0, 0);

	while (!quit)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:  x--; break;
			case SDLK_RIGHT: x++; break;
			case SDLK_UP:    y++; break;
			case SDLK_DOWN:  y--; break;
			}
			texture = updateTexture(sceneFile, *window, *renderer, size, x, y);
			break;

		case SDL_QUIT:
			quit = true;
			break;
		}

		// Clear the entire screen to our selected color.
		SDL_RenderClear(renderer);
		//SDL_Rect rect = { x, y, 64, 64 };
		SDL_RenderCopy(renderer, texture, NULL, NULL);// &rect);
		// Up until now everything was drawn behind the scenes.
		// This will show the new, red contents of the window.
		SDL_RenderPresent(renderer);
	}

	// cleanup SDL

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}