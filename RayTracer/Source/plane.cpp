#include "plane.h"

Tuple Plane::getLocalNormal(const Tuple& /*intersectionPoint*/) const
{
	// the plane is XZ plane
	return Tuple::Vector(0, 1, 0);
}

std::pair<bool, float> Plane::LocalIntersect(const Ray& ray) const
{
	// the plane is described as the XZ plane, with the normal pointing in the
	// positive y direction.
	// ray equation: p = p0 + t.v
	// we want y = 0 for the intersection point
	// p.y = p0.y + t*v.y
	// p0.y + t*v.y = 0
	// t = -p0.y / v.y

	float const yDirection = ray.getDirection().Y();
	// parallel or coplanar ray to the XZ plane
	if (isCloseToZero(yDirection))
	{
		return std::make_pair(false, 0.0f);
	}
	else
	{
		float t = -ray.getOrigin().Y() / yDirection;
		if (t > 0)
		{
			return std::make_pair(true, t);
		}
		else
		{
			return std::make_pair(false, 0.0f);
		}
	}
}