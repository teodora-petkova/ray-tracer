//------------------------------------------------------------
// object.h Header
//------------------------------------------------------------
#ifndef OBJECT_H
#define OBJECT_H

#include "vector3.h"
#include "ray.h"
#include "material.h"

class Object
{
public:
	Object() : objectName(0) {};

	void  SetName(const char *name)
	{
		objectName = (char*)name;
	}

	char *GetName()
	{
		return objectName;
	}

	void  SetMaterial(Material &material)
	{
		objectMaterial = material;
	}

	Material *GetMaterial()
	{
		return &objectMaterial;
	}

	virtual int GetType() = 0;

	virtual int Intersect(Ray &ray, float &distance) = 0;

	virtual Vector3 GetNormal(Vector3& pos) = 0;

protected:
	char * objectName;
	Material objectMaterial;
};

#endif