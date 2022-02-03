#pragma once

#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include "tuple.h"
#include "ray.h"
#include "object.h"

class RAYTRACER_EXPORT Cube : public virtual Object
{
public:
	// the cube is centered at (0, 0, 0) and its bounds for all x, y, z are [-1, 1]!

	Cube() : Object()
	{
		this->bounds = BoundingBox(Tuple::Point(-1, -1, -1), Tuple::Point(1, 1, 1));
		this->boundsInParentSpace = this->bounds;
	}

	Cube(MaterialPtr material, Matrix<4, 4> transformation, bool showShadows = true)
		: Object(material, transformation)
	{
		this->bounds = BoundingBox(Tuple::Point(-1, -1, -1), Tuple::Point(1, 1, 1));
		this->boundsInParentSpace = this->bounds.Transform(transformation);
	}

private:
	IntersectionParams LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const override;

	Tuple getLocalNormal(const Tuple& point, const IntersectionParams& intersection) const override;
};