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
#include <math.h>
#include <future>

using namespace ReadScene;

//------------------------------------------------------------
// Main Ray Tracing Function
//------------------------------------------------------------
Color rayTrace(Ray& ray, Scene& scene)
{
	float minDistance = INFINITY;

	Object* object = NULL;
	IntersectionInfo intersection = IntersectionInfo();
	Color color = 0.0;

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

	if (object != NULL)
	{
		for (Light light : scene.Lights)
		{
			Vector3 rayFromLightToIntersection = intersection.getIntersectionPoint() - light.Position; // P - Lp = lightDirection
			Vector3 rayFromIntersectionToLight = -rayFromLightToIntersection;

			float bias = 0.001f;
			Ray rayToLightSource = Ray(intersection.getIntersectionPoint() + intersection.getNormal() * bias, rayFromIntersectionToLight);
			bool isInShadow = false;
			for (unsigned int i = 0; i < scene.Objects.size(); i++)
			{
				Object* testObject = scene.Objects[i];
				IntersectionInfo intersection = testObject->intersect(rayToLightSource);
				if (intersection.isHit())
				{
					isInShadow = true;
					break;
				}
			}

			float defaultObjectAlbedo = 0.18f;
			Color ambientColor = object->getMaterial()->getColor() * defaultObjectAlbedo;
			Color lightIlumination = light.Colour * light.Intensity / (M_PI * 4.0f * rayFromLightToIntersection.magnitude());
			//rayFromIntersectionToLight /= rayFromIntersectionToLight.magnitude();
			rayFromLightToIntersection.normalize();
			if (isInShadow)
			{
				color += Color(0, 0, 0);
			}
			else
			{
				color += ambientColor * (fabs(fmax(0.0f, dot(intersection.getNormal(), -rayFromLightToIntersection)))) * lightIlumination;// / M_PI;
			}
		}
	}

	return color;
}

void updatePixels(int i, int j, unsigned char* pixels, Color& color, int width)
{
	int b = (int)(color.z * 255);
	int g = (int)(color.y * 255);
	int r = (int)(color.x * 255);
	int a = 1;
	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;

	//SDL - specific B G R A
	pixels[(j * width + i) * 4 + 0] = (int)b; // B
	pixels[(j * width + i) * 4 + 1] = (int)g; // G
	pixels[(j * width + i) * 4 + 2] = (int)r; // R
	pixels[(j * width + i) * 4 + 3] = (int)a; // alpha (opacity)
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