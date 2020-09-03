#include <iostream>
#include <Source\raytracer.h>
#include "SDL.h"

#include "ctime"

using namespace std;
//------------------------------------------------------------
//  Main Function
//------------------------------------------------------------
int main(int argc, char* argv[])
{
	bool quit = false;
	SDL_Event event;
	int x = 10;
	int y = 25;

	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		printf("SDL Init error: %s", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("SDL2 Displaying Image", // window's title
		10, 25, // coordinates on the screen, in pixels, of the window's upper left corner
		640, 480, // window's length and height in pixels
		SDL_WINDOW_OPENGL);

	// We must call SDL_CreateRenderer in order for draw calls to affect this window.
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	clock_t begin = clock();

	RayTracer r = RayTracer();
	r.execute(argv[1], argv[2]);

	clock_t end = clock();

	float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
	cout << elapsed_secs << '\n';

	SDL_Surface* image = SDL_LoadBMP(argv[2]);

	auto err = SDL_GetError();
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	SDL_FreeSurface(image);

	// Select the color for drawing. It is set to red here.
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	while (!quit)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
			//case SDLK_UP:
			//	r.execute();

		/*case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:  x--; break;
			case SDLK_RIGHT: x++; break;
			case SDLK_UP:    y--; break;
			case SDLK_DOWN:  y++; break;
			}
			break;
			*/
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