#pragma once

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

private:
	Tuple A;
	Tuple B;
	Tuple C;
	Tuple normal;

	void Initialize();
	bool IsPointInTriangleByBarycentricCoordinates(const Tuple& P) const;
	bool IsPointInTriangleByHalfPlanes(const Tuple& P) const;

	std::pair<bool, float> LocalIntersect(const Ray& ray) const override;
	Tuple getLocalNormal(const Tuple& ray) const override;
};