//------------------------------------------------------------
// object.h Header
//------------------------------------------------------------
#ifndef OBJECT_H
#define OBJECT_H

#include "vector3.h"
#include "ray.h"
#include "material.h"
#include "intersectioninfo.h"

class Object
{
public:
	Object() : objectName(0) {};

	void  setName(const char *name)
	{
		objectName = (char*)name;
	}

	char *getName()
	{
		return objectName;
	}

	void  setMaterial(Material &material)
	{
		objectMaterial = material;
	}

	Material *getMaterial()
	{
		return &objectMaterial;
	}

	virtual int getType() = 0;

	virtual IntersectionInfo intersect(Ray &ray) = 0;

	virtual Vector3 getNormal(Vector3& pos) = 0;

protected:
	char * objectName;
	Material objectMaterial;
};

#endif