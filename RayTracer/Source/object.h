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
	Object() :material(nullptr)
	{}

	Object(MaterialPtr material) : material(material)
	{}

	virtual IntersectionInfo Intersect(const Ray& ray) const = 0;

	MaterialPtr getMaterial() const { return material; }

protected:
	MaterialPtr material;
};

using ObjectPtr = std::shared_ptr<Object>;

#endif