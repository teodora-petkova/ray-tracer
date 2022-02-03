#pragma once

#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include "tuple.h"
#include "ray.h"
#include "object.h"

class RAYTRACER_EXPORT Sphere : public virtual Object
{
public:
	Sphere() : Object(),
		center(Tuple::Point(0, 0, 0)),
		radius(1)
	{
		this->bounds = BoundingBox(Tuple::Point(-1, -1, -1), Tuple::Point(1, 1, 1));
		this->boundsInParentSpace = this->bounds;
	}

	Sphere(const Tuple& center, float radius,
		MaterialPtr material, Matrix<4, 4> transformation) :
		Object(material, transformation),
		center(center),
		radius(radius)
	{
		this->bounds = BoundingBox(Tuple::Point(-1, -1, -1), Tuple::Point(1, 1, 1));
		this->boundsInParentSpace = this->bounds.Transform(transformation);
	}

	Tuple getCenter() const { return center; }
	float getRadius() const { return radius; }

private:
	Tuple center;
	float radius;

	IntersectionParams LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const override;
	Tuple getLocalNormal(const Tuple& point, const IntersectionParams& intersection) const override;

	float AlgebraicIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const;

	float GeometricIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const;

	float getIntersectionResult(const float& t1, const float& t2,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const;
};