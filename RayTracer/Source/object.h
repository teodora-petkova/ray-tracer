//------------------------------------------------------------
// object.h Header
//------------------------------------------------------------
#ifndef OBJECT_H
#define OBJECT_H

#include "Tuple.h"
#include "ray.h"
#include "material.h"
#include "intersectioninfo.h"

class __declspec(dllexport) Object
{
public:
	Object() {}

	void setMaterial(Material& material)
	{
		objectMaterial = material;
	}

	Material* getMaterial()
	{
		return &objectMaterial;
	}

	virtual IntersectionInfo intersect(Ray& ray) = 0;

protected:
	Material objectMaterial;
};

#endif