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
	{}

	Sphere(const Tuple& center, float radius,
		MaterialPtr material, Matrix<4, 4> transformation) :
		Object(material, transformation),
		center(center),
		radius(radius)
	{}

	Tuple getCenter() const { return center; }
	float getRadius() const { return radius; }

private:
	Tuple center;
	float radius;

	std::pair<bool, float> LocalIntersect(const Ray& ray) const override;
	Tuple getLocalNormal(const Tuple& ray) const override;
};