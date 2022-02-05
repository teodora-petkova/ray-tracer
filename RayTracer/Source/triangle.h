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
	Triangle(std::string name = "");
	Triangle(const Tuple& point1, const Tuple& point2, const Tuple& point3,
		MaterialPtr material, Matrix<4, 4> transformation, std::string name = "");

	Tuple getA() const { return A; }
	Tuple getB() const { return B; }
	Tuple getC() const { return C; }

	Tuple getAB() const { return AB; }
	Tuple getAC() const { return AC; }
	Tuple getTriangleNormal() const { return normal; }

	void Divide(int threshold) { }

	bool operator==(const Object& other) const;

private:
	Tuple A;
	Tuple B;
	Tuple C;

	Tuple AB;
	Tuple AC;
	Tuple BC;
	Tuple normal;

	void Initialize();
	bool IsPointInTriangleByBarycentricCoordinates(const Tuple& P) const;
	bool IsPointInTriangleByHalfPlanes(const Tuple& P) const;

	IntersectionParams LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const override;

	float LocalIntersect2(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const;

	Tuple getLocalNormal(const Tuple& point, const IntersectionParams& intersection) const override;
};