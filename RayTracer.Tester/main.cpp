#pragma warning(push, 0)
#include <iostream>
#include <ctime>
#include "SDL.h"
#pragma warning(pop)

#include "source\raytracer.h"
#include "source\scene.h"
#include "source\readfile.h"
#include "SDL\SDLApp.h"

unsigned char GetColorComponent(float colorComponentF)
{
	int colorComponent = int(colorComponentF * 255);

	if (colorComponent > 255) colorComponent = 255;

	return (unsigned char)colorComponent;
}

void UpdatePixels(const Canvas& canvas, unsigned char* pixels)
{
	int width = canvas.getWidth();

	for (int i = 0; i < canvas.getWidth(); i++)
	{
		for (int j = 0; j < canvas.getHeight(); j++)
		{
			Color color = canvas.PixelAt(i, j);
			//SDL - specific B G R A
			pixels[(j * width + i) * 4 + 0] = GetColorComponent(color.B());
			pixels[(j * width + i) * 4 + 1] = GetColorComponent(color.G());
			pixels[(j * width + i) * 4 + 2] = GetColorComponent(color.R());
			pixels[(j * width + i) * 4 + 3] = (unsigned char)1;
		}
	}
}

void UpdateWindow(SDLApp& window,
	Scene& scene, int size,
	int x, int y, const char* fileToSaveScene)
{
	clock_t begin = clock();

	// TODO: to think of a correct way to move the camera
	// scene.getModifiableCamera().setTransform(Transformations::Translation(x, y, 0));

	RayTracer rayTracer = RayTracer();
	Canvas canvas = rayTracer.TraceRays(scene);

	unsigned char* pixels = new unsigned char[scene.getImageWidth() * scene.getImageHeight() * 4];
	UpdatePixels(canvas, pixels);

	window.Update(pixels, size, fileToSaveScene);

	delete[] pixels;
	pixels = nullptr;

	clock_t end = clock();

	float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Elapsed time in seconds: " << elapsed_secs << " sec\n";
}

//------------------------------------------------------------
//  Main Function
//------------------------------------------------------------
int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cout << "No input scene file! Cannot load the current scene!" << std::endl;
		return 1;
	}

	const char* sceneFile = argv[1];
	Scene scene = ReadScene::ReadSceneFile(sceneFile);

	const char* fileToSaveScene = "";
	if (argc == 3)
	{
		fileToSaveScene = argv[2];
	}

	int x = 0;
	int y = 0;
	int rows = scene.getImageWidth();
	int columns = scene.getImageHeight();
	int size = rows * columns * sizeof(unsigned char) * 4;

	bool quit = false;
	SDL_Event event;

	try
	{
		SDLApp window("Simple Ray tracer", 10, 25, rows, columns);

		UpdateWindow(window, scene, size, x, y, fileToSaveScene);

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
				UpdateWindow(window, scene, size, x, y, fileToSaveScene);
				break;

			case SDL_QUIT:
				quit = true;
				break;
			}

			// just quickly show the window, when we save the image in a file (used for batch scripts)
			if (fileToSaveScene != "")
			{
				quit = true;
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}