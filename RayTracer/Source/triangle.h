//------------------------------------------------------------
// triangle.h Header
//------------------------------------------------------------
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cmath>
#include "vector3.h"
#include "ray.h"
#include "object.h"

#define TRIANGLE 2

class Triangle : public  Object
{
public:
	Triangle();

	Triangle(Vector3 &point1, Vector3 &point2, Vector3 &point3) :
		pointA(point1),
		pointB(point2),
		pointC(point3)
	{};

	int getType()
	{
		return TRIANGLE;
	}

	Vector3 getNormal(Vector3& pos)
	{
		Vector3 normal = cross((pointB - pointA), (pointC - pointA));
		normal.normalize();
		return normal;
	}
	IntersectionInfo intersect(Ray &ray);

private:
	Vector3 pointA;
	Vector3 pointB;
	Vector3 pointC;
};

#endif