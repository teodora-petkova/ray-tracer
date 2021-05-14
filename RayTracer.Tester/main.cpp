#pragma warning(push, 0)
#include <iostream>
#include <ctime>
#include "SDL.h"
#pragma warning(pop)

#include "source\raytracer.h"
#include "source\scene.h"
#include "source\readfile.h"
#include "SDL\SDLApp.h"

using namespace std;

void updateWindow(SDLApp& window,
	Scene& scene, int size,
	int x, int y)
{
	clock_t begin = clock();

	scene.Camera.updateLookAt(x, y);

	RayTracer r = RayTracer();
	unsigned char* pixels = r.execute(scene);
	window.Update(pixels, size);
	delete[] pixels;
	pixels = nullptr;

	clock_t end = clock();

	float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
	cout << elapsed_secs << '\n';
}

//------------------------------------------------------------
//  Main Function
//------------------------------------------------------------
int main(int argc, char* argv[])
{
	const char* sceneFile = argv[1];
	Scene scene = ReadScene::readSceneFile(sceneFile);

	int x = 0;
	int y = 0;
	int rows = scene.ImageWidth;
	int columns = scene.ImageHeight;
	int size = rows * columns * sizeof(unsigned char) * 4;

	bool quit = false;
	SDL_Event event;

	try
	{

		SDLApp window("Simple Ray tracer", 10, 25, rows, columns);

		updateWindow(window, scene, size, x, y);

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

				//printf("(x, y) = (%d, %d)\n", x, y);
				updateWindow(window, scene, size, x, y);
				break;

			case SDL_QUIT:
				quit = true;
				break;
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}