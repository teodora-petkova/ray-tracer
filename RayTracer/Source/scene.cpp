//------------------------------------------------------------
// scene.cpp
//------------------------------------------------------------

#include "scene.h"

using namespace std;

Scene::~Scene()
{
	objectsCount = 0;
	delete objects;
}
void Scene::initializeScene(vector<Triangle> triangles, vector<Sphere> spheres, vector<Light> lights)
{
	int trianglesNumber = triangles.size();
	int spheresNumber = spheres.size();
	objectsCount = trianglesNumber + spheresNumber;

	objects = new Object*[objectsCount];

	int i = 0;
	for (; i < trianglesNumber; i++)
	{
		objects[i] = new Triangle(triangles[i]);
		objects[i]->setName("Triangle" + i);
	}
	for (int j = 0; j < spheresNumber; j++, i++)
	{
		objects[i] = new Sphere(spheres[j]);
		objects[i]->setName("Sphere" + j);
	}

	this->lights = lights;
}