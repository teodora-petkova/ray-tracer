//------------------------------------------------------------
// Cscene.h Header
//------------------------------------------------------------
#ifndef SCENE_H
#define SCENE_H

#include "object.h"
#include <vector>
#include "sphere.h"
#include "triangle.h"

using namespace std;

class Scene
{
public:
	Scene() : objectsCount(0), objects(0) {};

	~Scene();

	void InitializeScene(vector<Triangle> triangles, vector<Sphere> spheres);

	int GetNumberOfObjects()
	{
		return objectsCount;
	}

	Object* GetObject(int index)
	{
		return objects[index];
	}

private:
	int objectsCount;
	Object** objects;
};

#endif