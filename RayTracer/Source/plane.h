#pragma once

#include "tuple.h"
#include "ray.h"
#include "object.h"
#include "utils.h"

class RAYTRACER_EXPORT Plane : public virtual Object
{
public:
	Plane() : Object()
	{}

	Plane(MaterialPtr material, Matrix<4, 4> transformation) :
		Object(material, transformation)
	{}

private:
	std::pair<bool, float> LocalIntersect(const Ray& ray) const override;
	Tuple getLocalNormal(const Tuple& ray) const override;
};