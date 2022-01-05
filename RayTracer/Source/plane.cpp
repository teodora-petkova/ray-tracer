#include "plane.h"

Tuple Plane::getLocalNormal(const Tuple& /*intersectionPoint*/) const
{
	// the plane is XZ plane
	return Tuple::Vector(0, 1, 0);
}

float Plane::LocalIntersect(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
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
		return -1.f;
	}
	else
	{
		float t = -ray.getOrigin().Y() / yDirection;
		if (t > 0)
		{
			intersectionDistances.emplace_back(std::make_pair(t, shared_from_this()));
			return t;
		}
		else
		{
			return -1.f;
		}
	}
}