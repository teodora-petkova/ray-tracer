//------------------------------------------------------------
// sphere.h Header
//------------------------------------------------------------
#ifndef SPHERE_H
#define SPHERE_H

#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include "tuple.h"
#include "ray.h"
#include "object.h"

class Sphere : public virtual Object
{
public:
	Sphere() : Object(material),
		center(Tuple::Point(0, 0, 0)),
		radius(1)
	{}

	Sphere(const Tuple& center, float radius,
		MaterialPtr material) :
		Object(material),
		center(center),
		radius(radius)
	{}

	IntersectionInfo Intersect(const Ray& ray) const override;

	Tuple getCenter() const { return center; }
	float getRadius() const { return radius; }

private:
	Tuple center;
	float radius;
};

#endif