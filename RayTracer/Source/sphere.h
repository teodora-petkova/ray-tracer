//------------------------------------------------------------
// sphere.h Header
//------------------------------------------------------------
#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include "vector3.h"
#include "ray.h"
#include "object.h"

#define SPHERE 1

class Sphere : public  Object
{
public:
	Sphere();

	Sphere(const Vector3 &center, float radius) :
		sphereCenter(center),
		sphereRadius(radius),
		sphereRadiusSquared(radius*radius),
		sphereRadiusInverse(1.0f / radius) {};

	int getType()
	{
		return SPHERE;
	}

	Vector3 getCenter()
	{
		return sphereCenter;
	}

	float getRadius()
	{
		return sphereRadius;
	}

	float getRadiusSq()
	{
		return sphereRadiusSquared;
	}

	Vector3 getNormal(Vector3 &position)
	{
		return (position - sphereCenter)*sphereRadiusInverse;
	}

	IntersectionInfo intersect(Ray &ray);

private:
	Vector3 sphereCenter;
	float sphereRadius;
	float sphereRadiusSquared;
	float sphereRadiusInverse;
};

#endif