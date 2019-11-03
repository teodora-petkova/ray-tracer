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
#include "scenedata.h"
#include "scene.h"
#include <math.h>

#include "FreeImage.h"

using namespace ReadScene;

//------------------------------------------------------------
// Main Ray Tracing Function
//------------------------------------------------------------
Color rayTrace(Ray &ray, Scene &scene)
{
	float minDistance = INFINITY;

	Object *object = NULL;
	Vector3 intersectionPoint = NULL;
	Vector3 normal = NULL;
	Color color = 0.0;

	for (int i = 0; i < scene.getNumberOfObjects(); i++)
	{
		Object* testObject = scene.getObject(i);
		IntersectionInfo intersection = testObject->intersect(ray);
		if (intersection.isHit() && testObject && intersection.getDistance() < minDistance)
		{
			object = testObject;
			minDistance = intersection.getDistance();
			intersectionPoint = ray.getOrigin() + ray.getDirection() * intersection.getDistance();
			normal = intersection.getNormal();
		}
	}

	for (Light light : scene.getLights())
	{
		Vector3 rayFromLightToIntersection = intersectionPoint - light.Position; // P - Lp = lightDirection
		Vector3 rayFromIntersectionToLight = -rayFromLightToIntersection;
		if (object != NULL)
		{
			float bias = 0.001f;
			Ray rayToLightSource = Ray(intersectionPoint + normal * bias, rayFromIntersectionToLight);
			bool isInShadow = false;
			for (int i = 0; i < scene.getNumberOfObjects(); i++)
			{
				Object* testObject = scene.getObject(i);
				IntersectionInfo intersection = testObject->intersect(rayToLightSource);
				if (intersection.isHit())
				{
					isInShadow = true;
					break;
				}
			}

			float defaultObjectAlbedo = 0.18;
			Color ambientColor = object->getMaterial()->getColor() * defaultObjectAlbedo;
			if (isInShadow)
			{
				color = Color(0, 0, 0);
			}
			else
			{
				color = ambientColor * (fabs(fmax(0.0f, dot(normal, rayFromIntersectionToLight)))) * light.Colour * light.Intensity / M_PI;
			}
		}
	}

	return color;
}

void updatePixels(int i, int j, BYTE* pixels, Color& color, int width)
{
	int b = (int)(color.z * 255);
	int g = (int)(color.y * 255);
	int r = (int)(color.x * 255);
	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;

	//Free Image Library - specific B G R
	pixels[(j*width + i) * 3 + 0] = (int)b; // B
	pixels[(j*width + i) * 3 + 1] = (int)g; // G
	pixels[(j*width + i) * 3 + 2] = (int)r; // R
}

//------------------------------------------------------------
// Free Image
//------------------------------------------------------------
void saveScreenshot(std::string fname, int w, int h, BYTE* pixels)
{
	FreeImage_Initialise();

	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

	FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);

	FreeImage_DeInitialise();
}

//------------------------------------------------------------
//  Main Function
//------------------------------------------------------------
void RayTracer::execute(std::string sceneFileName, std::string outputImageFileName)
{
	SceneData sceneData = readSceneFile(sceneFileName);

	Camera camera = Camera(sceneData.LookFrom, sceneData.LookAt, sceneData.Up, sceneData.FovY, sceneData.ImageWidth, sceneData.ImageHeight);

	Scene scene = Scene();
	scene.initializeScene(sceneData.Triangles, sceneData.Spheres, sceneData.Lights);

	int pixelsSize = sceneData.ImageWidth * sceneData.ImageHeight;
	BYTE *pixels = new BYTE[pixelsSize * 3];

	for (int j = 0; j < sceneData.ImageHeight; j++)
	{
		for (int i = 0; i < sceneData.ImageWidth; i++)
		{
			Ray rayFromCamera = Ray(camera.getOrigin(), camera.getDirectionRayForPixel(i, j));

			Color color = rayTrace(rayFromCamera, scene);

			// the (0,0) point is at bottom-left!
			updatePixels(i, sceneData.ImageHeight - 1 - j, pixels, color, sceneData.ImageWidth);
		}
	}

	saveScreenshot(outputImageFileName, sceneData.ImageWidth, sceneData.ImageHeight, pixels);

	delete pixels;
}
