//------------------------------------------------------------
// Cscene.cpp
//------------------------------------------------------------

#include "scene.h"

Scene::~Scene()
{
	objectsNumber = 0;
	delete pvt_object;
}
void Scene::InitializeScene()
{
	int trianglesNumber = triangles.size();
	int spheresNumber = spheres.size();
	objectsNumber = trianglesNumber + spheresNumber;

	pvt_object = new Object*[objectsNumber];

	int i = 0;
	for (; i < trianglesNumber; i++)
	{
		pvt_object[i] = &triangles[i];
		pvt_object[i]->SetName("Triangle" + i);
	}
	for (int j = 0; j < spheresNumber; j++, i++)
	{
		pvt_object[i] = &spheres[j];
		pvt_object[i]->SetName("Sphere" + j);
	}
}