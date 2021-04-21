//------------------------------------------------------------
// sphere.h Header
//------------------------------------------------------------
#ifndef SPHERE_H
#define SPHERE_H

#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include "tuple.h"
#include "ray.h"
#include "object.h"

class Sphere : public  Object
{
public:
	Sphere();

	Sphere(const Tuple& center, float radius) :
		center(center),
		radius(radius) {}

	Tuple getCenter()
	{
		return center;
	}

	float getRadius()
	{
		return radius;
	}

	IntersectionInfo intersect(Ray& ray);

private:
	Tuple center;
	float radius;
};

#endif