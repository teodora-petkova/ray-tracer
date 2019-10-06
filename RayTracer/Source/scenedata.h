//------------------------------------------------------------
// Cscene.h Header
//------------------------------------------------------------
#ifndef SCENEDATA_H
#define SCENEDATA_H

#include "vector3.h"
#include "sphere.h"
#include "triangle.h"
#include <vector>

using namespace std;

class SceneData
{
public:
	SceneData()
	{
		this->LookFrom = Vector3(0.0, 0.0, 5.0); // Initial eye position, also for resets
		this->LookAt = Vector3(0.0, 1.0, 0.0); // Initial up position, also for resets
		this->Up = Vector3(0.0, 0.0, 0.0); // Center look at point 
		this->LightPosition = Vector3(0.0, 0.0, 0.0);
		this->LightColour = Vector3(0.0, 0.0, 0.0);
		this->ImageWidth = 600;
		this->ImageHeight = 400;
		this->FovY = 90.0; // For field of view
		this->Vertices = vector<Vector3>();
		this->Triangles = vector<Triangle>();
		this->Spheres = vector<Sphere>();
	}

	~SceneData();

	// image size
	int ImageWidth;
	int ImageHeight;

	// camera
	Vector3 LookFrom;
	Vector3 LookAt;
	Vector3 Up;
	float FovY;

	//light
	Vector3 LightPosition;
	Vector3 LightColour;

	// objects
	vector<Vector3> Vertices;
	vector<Triangle> Triangles;
	vector<Sphere> Spheres;
};

#endif