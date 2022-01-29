#pragma once

#include "tuple.h"
#include "ray.h"
#include "object.h"
#include "utils.h"

class RAYTRACER_EXPORT Plane : public virtual Object
{
public:
	Plane() : Object()
	{
		this->bounds = BoundingBox(
			Tuple::Point(-INFINITY, 0, -INFINITY),
			Tuple::Point(INFINITY, 0, INFINITY));
	}

	Plane(MaterialPtr material, Matrix<4, 4> transformation) :
		Object(material, transformation)
	{
		this->bounds = BoundingBox(
			Tuple::Point(-INFINITY, 0, -INFINITY),
			Tuple::Point(INFINITY, 0, INFINITY))
			.Transform(this->transformation);
	}

private:
	IntersectionParams LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const override;
	Tuple getLocalNormal(const Tuple& point, const IntersectionParams& intersection) const override;
};