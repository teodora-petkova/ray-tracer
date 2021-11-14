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

class RAYTRACER_EXPORT Sphere : public virtual Object
{
public:
	Sphere() : Object(material, transformation),
		center(Tuple::Point(0, 0, 0)),
		radius(1)
	{}

	Sphere(const Tuple& center, float radius,
		MaterialPtr material, Matrix<4, 4> transformation) :
		Object(material, transformation),
		center(center),
		radius(radius)
	{}


	IntersectionInfo Intersect(const Ray& ray) const override;

	Tuple getCenter() const { return center; }
	float getRadius() const { return radius; }
	Tuple getNormal(Tuple intersectionPoint) const;

private:
	Tuple center;
	float radius;
};

#endif