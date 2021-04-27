//------------------------------------------------------------
// triangle.h Header
//------------------------------------------------------------
#ifndef TRIANGLE_H
#define TRIANGLE_H

#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "Tuple.h"
#include "ray.h"
#include "object.h"

class RAYTRACER_EXPORT Triangle : public  Object
{
public:
	Triangle();

	Triangle(Tuple& point1, Tuple& point2, Tuple& point3);

	IntersectionInfo intersect(Ray& ray);

private:
	Tuple A;
	Tuple B;
	Tuple C;
	Tuple normal;

	void initialize();
	bool isPointInTriangleByBarycentricCoordinates(Tuple P);
	bool isPointInTriangleByHalfPlanes(Tuple P);
};

#endif