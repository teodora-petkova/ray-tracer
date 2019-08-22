//------------------------------------------------------------
// Cscene.h Header
//------------------------------------------------------------
#ifndef CSCENE_H
#define CSCENE_H

#include "vector3.h"
#include "sphere.h"
#include "triangle.h"
#include "ray.h"
#include "object.h"
#include "variables.h"

class Scene
{
public:
	Scene() : objectsNumber(0), pvt_object(0) {};

	~Scene();

	void InitializeScene();

	int GetNumberOfObjects()
	{
		return objectsNumber;
	}

	Object *GetObject(int index)
	{
		return pvt_object[index];
	}

private:
	int objectsNumber;
	Object **pvt_object;
};

#endif