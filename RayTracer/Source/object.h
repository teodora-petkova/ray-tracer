//------------------------------------------------------------
// object.h Header
//------------------------------------------------------------
#ifndef OBJECT_H
#define OBJECT_H

#include "raytracer_exports.h"
#include "tuple.h"
#include "ray.h"
#include "material.h"
#include "intersectioninfo.h"

class RAYTRACER_EXPORT Object
{
public:
	Object() {}

	void setMaterial(MaterialPtr material)
	{
		objectMaterial = material;
	}

	MaterialPtr getMaterial()
	{
		return objectMaterial;
	}

	virtual IntersectionInfo intersect(Ray& ray) = 0;

protected:
	MaterialPtr objectMaterial;
};

using ObjectPtr = std::shared_ptr<Object>;

#endif