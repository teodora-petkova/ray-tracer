//------------------------------------------------------------
// Cscene.h Header
//------------------------------------------------------------
#ifndef SCENE_H
#define SCENE_H

#include "Tuple.h"
#include "light.h"
#include "sphere.h"
#include "triangle.h"
#include "camera.h"
#include <vector>

using namespace std;

class __declspec(dllexport) Scene
{
public:
	Scene()
	{
		this->ImageWidth = 600;
		this->ImageHeight = 400;

		// Camera
		Tuple lookFrom = Tuple::Vector(0.0, 0.0, 5.0); // initial eye position, also for resets
		Tuple lookAt = Tuple::Vector(0.0, 1.0, 0.0); // initial up position, also for resets
		Tuple up = Tuple::Vector(0.0, 0.0, 0.0); // center look at point 
		float fovY = 90.0; // field of view
		this->Camera = Camera::Camera(lookFrom, lookAt, up, fovY,
			this->ImageWidth, this->ImageHeight);

		// Objects
		this->Objects = vector<Object*>();

		// lights
		this->Lights = vector<Light*>();
	}

	Scene(int width, int height, Camera camera,
		vector<Object*> objects, vector<Light*> lights)
	{
		this->ImageWidth = width;
		this->ImageHeight = height;
		this->Camera = camera;
		this->Objects = objects;
		this->Lights = lights;
	}

	~Scene()
	{
	}

	// image size
	int ImageWidth;
	int ImageHeight;

	// camera
	Camera Camera;

	// Objects
	vector<Object*> Objects;

	//Lights
	vector<Light*> Lights;
};

#endif