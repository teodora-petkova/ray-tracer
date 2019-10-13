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

#include "readfile.h"
#include "scenedata.h"
#include "scene.h"
#include "ctime"
#include <math.h>

using namespace std;
using namespace ReadScene;

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
Color rayTrace(Ray &ray, Scene &scene, Vector3 lightPosition)
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
		if (intersection.IsHit() && testObject && intersection.GetDistance() < minDistance)
		{
			object = testObject;
			minDistance = intersection.GetDistance();
			intersectionPoint = ray.GetOrigin() + ray.GetDirection() * intersection.GetDistance();
			normal = intersection.GetNormal();
		}
	}

	Vector3 rayFromIntersectionToLight = lightPosition - intersectionPoint;
	if (object != NULL)
	{
		Ray rayToLightSource = Ray(intersectionPoint + normal * 0.001f, rayFromIntersectionToLight);
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
			color = object->GetMaterial()->GetColor() *(fmax(0.0f, Dot(normal, -rayFromIntersectionToLight)));// *M_PI;
		}
		else
		{
			color = object->GetMaterial()->GetColor() *(fmax(0.0f, Dot(normal, -rayFromIntersectionToLight)));// *M_PI;
			//	color = Vector3(1.0, 1.0, 1.0);
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
//  Main Function
//------------------------------------------------------------
int main(int argc, char *argv[])
{
	clock_t begin = clock();

	SceneData sceneData = ReadSceneFile(argv[1]);

	Camera camera = Camera(sceneData.LookFrom, sceneData.LookAt, sceneData.Up, sceneData.FovY, sceneData.ImageWidth, sceneData.ImageHeight);

	Scene scene = Scene();
	scene.InitializeScene(sceneData.Triangles, sceneData.Spheres);

	int pixelsSize = sceneData.ImageWidth * sceneData.ImageHeight;
	BYTE *pixels = new BYTE[pixelsSize * 3];

	for (int i = 0; i < sceneData.ImageWidth; i++)
	{
		for (int j = 0; j < sceneData.ImageHeight; j++)
		{
			Ray rayFromCamera = Ray(camera.GetOrigin(), camera.GetDirectionRayForPixel(i, j));

			Color color = rayTrace(rayFromCamera, scene, sceneData.LightPosition);

			// the (0,0) point is at bottom-left!
			updatePixels(i, sceneData.ImageHeight - 1 - j, pixels, color, sceneData.ImageWidth);
		}
	}

	saveScreenshot(argv[2], sceneData.ImageWidth, sceneData.ImageHeight, pixels);

	delete pixels;

	clock_t end = clock();
	float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
	cout << elapsed_secs << '\n';

	return 0;
}
