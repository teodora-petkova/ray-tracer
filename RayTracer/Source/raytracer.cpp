//------------------------------------------------------------
// RayTracer.cpp
//------------------------------------------------------------

#pragma warning(push, 0)
#include <math.h>
#include <future>
#include <thread>
#pragma warning(pop)

#include "raytracer.h"
#include "ray.h"
#include "sphere.h"
#include "material.h"
#include "camera.h"
#include "readfile.h"
#include "scene.h"
#include "transformations.h"

#define MAINPROGRAM 
#define _USE_MATH_DEFINES

using namespace ReadScene;

//------------------------------------------------------------
// Main Ray Tracing Function
//------------------------------------------------------------


//Canvas RayTracer::TraceRays(Scene& scene)
//{
//	vector<future<void>> futureCalculations;
//	int numCores = thread::hardware_concurrency();
//
//	Canvas canvas = Canvas(scene.ImageWidth, scene.ImageHeight);
//	int imageDims = scene.ImageWidth * scene.ImageHeight;
//
//	atomic<int> count(0);
//	while (numCores--)
//	{
//		futureCalculations.push_back(std::async(
//			[&count, &imageDims, &canvas, &scene]()
//			{
//				while (true)
//				{
//					int index = count++;
//					if (index >= imageDims)
//						break;
//					int x = index % scene.ImageWidth;
//					int y = index / scene.ImageWidth;
//
//					Ray rayFromCamera = Ray(scene.Camera.getOrigin(),
//						scene.Camera.getDirectionRayForPixel(x, y));
//
//					Color color = TraceSingleRay(rayFromCamera, scene);
//
//					// the (0,0) point is at bottom-left!
//					canvas.WritePixel(x, y, color);
//				}}
//		));
//	}
//
//	for (auto&& f : futureCalculations)
//	{
//		f.get();
//	}
//
//	return canvas;
//}

//------------------------------------------------------------
//  Main Function
//------------------------------------------------------------

void RayTracePixelChunks(int threadNum, int chunkSize, const Scene& scene, Canvas* canvas)
{
	int width = scene.getImageWidth();

	int heightStart = threadNum * chunkSize;
	int heightEnd = (threadNum + 1) * chunkSize;
	heightEnd = heightEnd < scene.getImageHeight() ? heightEnd : scene.getImageHeight();

	for (int x = 0; x < width; x++)
	{
		for (int y = heightStart; y < heightEnd; y++)
		{
			Ray rayFromCamera = scene.getCamera().CalculateRayForPixel(x, y);

			Color color = scene.TraceSingleRay(rayFromCamera);

			canvas->WritePixel(x, y, color);
		}
	}
}

Canvas RayTracer::TraceRays(const Scene& scene) const
{
	Canvas canvas = Canvas(scene.getImageWidth(), scene.getImageHeight());

	int numThreads = std::thread::hardware_concurrency();
	int chunkSize = scene.getImageHeight() > numThreads ?
		std::ceilf(scene.getImageHeight() / float(numThreads)) : 1;

	std::vector<std::thread> threads(numThreads);

	for (int i = 0; i < numThreads; i++)
	{
		threads[i] = std::thread(RayTracePixelChunks, i, chunkSize, scene, &canvas);
	}

	for (std::thread& t : threads)
	{
		t.join();
	}

	return canvas;
}
