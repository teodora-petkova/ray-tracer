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

class RAYTRACER_EXPORT Triangle : public virtual Object
{
public:
	Triangle();
	Triangle(const Tuple& point1, const Tuple& point2, const Tuple& point3,
		MaterialPtr material, Matrix<4, 4> transformation);

	IntersectionInfo Intersect(const Ray& ray) const override;

private:
	Tuple A;
	Tuple B;
	Tuple C;
	Tuple normal;

	void Initialize();
	bool IsPointInTriangleByBarycentricCoordinates(const Tuple& P) const;
	bool IsPointInTriangleByHalfPlanes(const Tuple& P) const;
};

#endif