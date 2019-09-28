//------------------------------------------------------------
// RayTracing Test Program
//------------------------------------------------------------
#include <iostream>

#include "ray.h"
#include "sphere.h"
#include "material.h"
#include "camera.h"
#include "FreeImage.h"

#define MAINPROGRAM 
#define _USE_MATH_DEFINES

#include "variables.h"
#include "readfile.h"
#include "scene.h"
#include "ctime"
#include <math.h>

using namespace std;
using namespace readfiles;

//------------------------------------------------------------
// Free Image
//------------------------------------------------------------
void saveScreenshot(string fname, int w, int h, BYTE *pixels)
{
	FreeImage_Initialise();

	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

	FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);

	FreeImage_DeInitialise();
}

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

	for (int i = 0; i < scene.GetNumberOfObjects(); i++)
	{
		Object* testObject = scene.GetObject(i);
		IntersectionInfo intersection = testObject->Intersect(ray);
		if (intersection.IsHit() && testObject && intersection.getDistance() < minDistance)
		{
			object = testObject;
			minDistance = intersection.getDistance();
			intersectionPoint = ray.GetOrigin() + ray.GetDirection() * intersection.getDistance();
			normal = intersection.getNormal();
		}
	}

	Vector3 rayFromIntersectionToLight = lightPosition - intersectionPoint;
	if (object != NULL)
	{
		Ray rayToLightSource = Ray(intersectionPoint + normal * 0.001, rayFromIntersectionToLight);
		bool isInShadow = false;
		for (int i = 0; i < scene.GetNumberOfObjects(); i++)
		{
			Object* testObject = scene.GetObject(i);
			IntersectionInfo intersection = testObject->Intersect(rayToLightSource);
			if (intersection.IsHit())
			{
				isInShadow = true;
				break;
			}

		}
		if (isInShadow)
		{
			color = object->GetMaterial()->GetColor() *(fmax(0.0, Dot(normal, -rayFromIntersectionToLight)));// *M_PI;
		}
		else
		{
			color = object->GetMaterial()->GetColor() *(fmax(0.0, Dot(normal, -rayFromIntersectionToLight)));// *M_PI;
			//	color = Vector3(1.0, 1.0, 1.0);
		}
	}

	return color;
}

void updatePixels(int i, int j, BYTE* pixels, Color& color)
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
//  Main Function
//------------------------------------------------------------
int main(int argc, char *argv[])
{
	clock_t begin = clock();

	readfile(argv[1]);

	Camera camera = Camera(lookfrom, lookat, up, fovy, width, height);

	Scene scene = Scene();
	scene.InitializeScene();

	int pixelsSize = width * height;
	BYTE *pixels = new BYTE[pixelsSize * 3];

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Ray rayFromCamera = Ray(camera.GetOrigin(), camera.GetDirectionRayForPixel(j, i));

			Color color = rayTrace(rayFromCamera, scene);

			updatePixels(i, j, pixels, color);
		}
	}

	saveScreenshot(argv[2], width, height, pixels);

	delete pixels;

	clock_t end = clock();
	float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
	cout << elapsed_secs << '\n';

	return 0;
}
