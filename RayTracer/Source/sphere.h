//------------------------------------------------------------
// sphere.h Header
//------------------------------------------------------------
#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include "vector3.h"
#include "ray.h"
#include "object.h"

class Sphere : public  Object
{
public:
	Sphere();

	Sphere(const Vector3& center, float radius) :
		center(center),
		radius(radius) {}

	Vector3 getCenter()
	{
		return center;
	}

	float getRadius()
	{
		return radius;
	}

	IntersectionInfo intersect(Ray& ray);

private:
	Vector3 center;
	float radius;
};

#endif