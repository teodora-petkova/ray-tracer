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

bool IsInShadow(IntersectionInfo intersection, Tuple lightPosition,
	std::vector<ObjectPtr> objects)
{
	// P - Lp = lightDirection
	Tuple rayFromLightToIntersection = intersection.getIntersectionPoint() - lightPosition;

	Tuple rayFromIntersectionToLight = -rayFromLightToIntersection;

	float bias = 0.001f;
	Ray rayToLightSource = Ray(intersection.getIntersectionPoint() + intersection.getNormal() * bias, rayFromIntersectionToLight);
	bool isInShadow = false;
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		ObjectPtr testObject = objects[i];
		IntersectionInfo testIntersection = testObject->intersect(rayToLightSource);
		if (testIntersection.isHit())
		{
			isInShadow = true;
			break;
		}
	}

	return isInShadow;
}

Color TraceSingleRay(Ray& ray, Scene& scene)
{
	ObjectPtr object = NULL;
	IntersectionInfo intersection = IntersectionInfo();

	float minDistance = INFINITY;
	for (unsigned int i = 0; i < scene.Objects.size(); i++)
	{
		ObjectPtr testObject = scene.Objects[i];
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
		for (LightPtr light : scene.Lights)
		{
			if (IsInShadow(intersection, light->getPosition(), scene.Objects))
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

//------------------------------------------------------------
//  Main Function
//------------------------------------------------------------
Canvas RayTracer::TraceRays(Scene& scene)
{
	vector<future<void>> futureCalculations;
	int num_cores = thread::hardware_concurrency();

	Canvas canvas = Canvas(scene.ImageWidth, scene.ImageHeight);
	int imageDims = scene.ImageWidth * scene.ImageHeight;

	atomic<int> count(0);
	while (num_cores--)
	{
		futureCalculations.push_back(std::async([&count, &imageDims, &canvas, &scene]()
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

					Color color = TraceSingleRay(rayFromCamera, scene);

					// the (0,0) point is at bottom-left!
					canvas.WritePixel(x, y, color);
				}}
		));
	}

	for (auto&& f : futureCalculations)
	{
		f.get();
	}

	return canvas;
}