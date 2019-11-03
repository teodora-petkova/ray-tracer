//------------------------------------------------------------
// Cscene.h Header
//------------------------------------------------------------
#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include <vector>
#include "light.h"
#include "sphere.h"
#include "triangle.h"

using namespace std;

class Scene
{
public:
	Scene() : objectsCount(0), objects(0) {};

	~Scene();

	void initializeScene(vector<Triangle> triangles, vector<Sphere> spheres, vector<Light> lights);

	int getNumberOfObjects()
	{
		return objectsCount;
	}

	Object* getObject(int index)
	{
		return objects[index];
	}

	vector<Light> getLights()
	{
		return this->lights;
	}
private:
	int objectsCount;
	Object** objects;
	vector<Light> lights;
};

#endif