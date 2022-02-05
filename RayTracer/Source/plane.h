#pragma once

#include "tuple.h"
#include "ray.h"
#include "object.h"
#include "utils.h"

class RAYTRACER_EXPORT Plane : public virtual Object
{
public:
	Plane(std::string name = "") : Object(name)
	{
		this->bounds = BoundingBox(
			Tuple::Point(-INFINITY, 0, -INFINITY),
			Tuple::Point(INFINITY, 0, INFINITY));
		this->boundsInParentSpace = this->bounds;
	}

	Plane(MaterialPtr material, Matrix<4, 4> transformation, std::string name = "") :
		Object(material, transformation, name)
	{
		this->bounds = BoundingBox(
			Tuple::Point(-INFINITY, 0, -INFINITY),
			Tuple::Point(INFINITY, 0, INFINITY));
		this->boundsInParentSpace = this->bounds.Transform(transformation);
	}

	void Divide(int threshold) { }

private:
	IntersectionParams LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const override;
	Tuple getLocalNormal(const Tuple& point, const IntersectionParams& intersection) const override;
};