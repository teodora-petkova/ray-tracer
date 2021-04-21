//------------------------------------------------------------
// RayTracer.cpp
//------------------------------------------------------------

#include "raytracer.h"
#include "ray.h"
#include "sphere.h"
#include "material.h"
#include "camera.h"

#define MAINPROGRAM 
#define _USE_MATH_DEFINES

#include "readfile.h"
#include "scene.h"

#pragma warning(push, 0)
#include <math.h>
#include <future>
#pragma warning(pop)

using namespace ReadScene;

//------------------------------------------------------------
// Main Ray Tracing Function
//------------------------------------------------------------

bool isInShadow(IntersectionInfo intersection, Tuple lightPosition,
	std::vector<Object*> objects)
{
	// P - Lp = lightDirection
	Tuple rayFromLightToIntersection = intersection.getIntersectionPoint() - lightPosition;

	Tuple rayFromIntersectionToLight = -rayFromLightToIntersection;

	float bias = 0.001f;
	Ray rayToLightSource = Ray(intersection.getIntersectionPoint() + intersection.getNormal() * bias, rayFromIntersectionToLight);
	bool isInShadow = false;
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		Object* testObject = objects[i];
		IntersectionInfo testIntersection = testObject->intersect(rayToLightSource);
		if (testIntersection.isHit())
		{
			isInShadow = true;
			break;
		}
	}

	return isInShadow;
}

Color rayTrace(Ray& ray, Scene& scene)
{
	Object* object = NULL;
	IntersectionInfo intersection = IntersectionInfo();

	float minDistance = INFINITY;
	for (unsigned int i = 0; i < scene.Objects.size(); i++)
	{
		Object* testObject = scene.Objects[i];
		IntersectionInfo testIntersection = testObject->intersect(ray);
		if (testIntersection.isHit() && testObject && testIntersection.getDistance() < minDistance)
		{
			object = testObject;
			intersection = testIntersection;
			minDistance = intersection.getDistance();
		}
	}

	Color color = Color(0, 0, 0);
	if (intersection.isHit())
	{
		for (Light* light : scene.Lights)
		{
			if (isInShadow(intersection, light->getPosition(), scene.Objects))
			{
				color += Color(0, 0, 0);
			}
			else
			{
				color += light->getPhongColor(intersection.getIntersectionPoint(),
					intersection.getNormal().normalize(),
					scene.Camera.getOrigin().normalize(),
					object->getMaterial());
			}
		}
	}
	return color;
}

void updatePixels(int i, int j, unsigned char* pixels, Color& color, int width)
{
	int b = int(color.z * 255);
	int g = int(color.y * 255);
	int r = int(color.x * 255);
	int a = 1;

	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;

	//SDL - specific B G R A
	pixels[(j * width + i) * 4 + 0] = b;
	pixels[(j * width + i) * 4 + 1] = g;
	pixels[(j * width + i) * 4 + 2] = r;
	pixels[(j * width + i) * 4 + 3] = a;
}

//------------------------------------------------------------
//  Main Function
//------------------------------------------------------------
unsigned char* RayTracer::execute(Scene scene)
{
	vector<future<void>> futures;

	int imageDims = scene.ImageWidth * scene.ImageHeight;
	unsigned char* pixels = new unsigned char[imageDims * 4];

	int num_cores = thread::hardware_concurrency();
	volatile atomic<int> count(0);
	while (num_cores--)
	{
		futures.push_back(async(launch::async, [&count, &imageDims, &pixels, &scene]()
			{
				while (true)
				{
					int index = count++;
					if (index >= imageDims)
						break;
					int x = index % scene.ImageWidth;
					int y = index / scene.ImageWidth;

					Ray rayFromCamera = Ray(scene.Camera.getOrigin(),
						scene.Camera.getDirectionRayForPixel(x, y));

					Color color = rayTrace(rayFromCamera, scene);

					// the (0,0) point is at bottom-left!
					updatePixels(x, y, pixels, color, scene.ImageWidth);
				}
			}));
	}

	return pixels;
}