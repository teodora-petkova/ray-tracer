//------------------------------------------------------------
// triangle.h Header
//------------------------------------------------------------
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cmath>
#include "Tuple.h"
#include "ray.h"
#include "object.h"

class Triangle : public  Object
{
public:
	Triangle();

	Triangle(Tuple& point1, Tuple& point2, Tuple& point3) :
		pointA(point1),
		pointB(point2),
		pointC(point3)
	{};

	IntersectionInfo intersect(Ray& ray);

private:
	Tuple pointA;
	Tuple pointB;
	Tuple pointC;
};

#endif