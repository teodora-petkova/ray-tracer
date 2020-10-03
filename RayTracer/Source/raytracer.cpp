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

using namespace ReadScene;

//------------------------------------------------------------
// Main Ray Tracing Function
//------------------------------------------------------------
Color rayTrace(Ray& ray, Scene& scene)
{
	float minDistance = INFINITY;

	Object* object = NULL;
	Vector3 intersectionPoint = NULL;
	Vector3 normal = NULL;
	Color color = 0.0;

	for (unsigned int i = 0; i < scene.Objects.size(); i++)
	{
		Object* testObject = scene.Objects[i];
		IntersectionInfo intersection = testObject->intersect(ray);
		if (intersection.isHit() && testObject && intersection.getDistance() < minDistance)
		{
			object = testObject;
			minDistance = intersection.getDistance();
			intersectionPoint = ray.getOrigin() + ray.getDirection() * intersection.getDistance();
			normal = intersection.getNormal();
		}
	}

	if (object != NULL)
	{
		for (Light light : scene.Lights)
		{
			Vector3 rayFromLightToIntersection = intersectionPoint - light.Position; // P - Lp = lightDirection
			Vector3 rayFromIntersectionToLight = -rayFromLightToIntersection;

			float bias = 0.001f;
			Ray rayToLightSource = Ray(intersectionPoint + normal * bias, rayFromIntersectionToLight);
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

			double defaultObjectAlbedo = 0.18;
			Color ambientColor = object->getMaterial()->getColor() * defaultObjectAlbedo;
			Color lightIlumination = light.Colour * light.Intensity / (M_PI * 4 * rayFromLightToIntersection.magnitude());
			//rayFromIntersectionToLight /= rayFromIntersectionToLight.magnitude();
			rayFromLightToIntersection.normalize();
			if (isInShadow)
			{
				color += Color(0, 0, 0);
			}
			else
			{
				color += ambientColor * (fabs(fmax(0.0f, dot(normal, -rayFromLightToIntersection)))) * lightIlumination;// / M_PI;
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
void RayTracer::execute(std::string sceneFileName)
{
	Scene scene = readSceneFile(sceneFileName);

	int pixelsSize = scene.ImageWidth * scene.ImageHeight;
	unsigned char* pixels = new unsigned char[pixelsSize * 4];

	for (int j = 0; j < scene.ImageHeight; j++)
	{
		for (int i = 0; i < scene.ImageWidth; i++)
		{
			Ray rayFromCamera = Ray(scene.Camera.getOrigin(),
				scene.Camera.getDirectionRayForPixel(i, j));

			Color color = rayTrace(rayFromCamera, scene);

			// the (0,0) point is at bottom-left!
			updatePixels(i, j, pixels, color, scene.ImageWidth);
		}
	}

	delete[] pixels;
}