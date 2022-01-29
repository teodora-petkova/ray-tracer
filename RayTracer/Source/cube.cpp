#include "cube.h"

Tuple Cube::getLocalNormal(const Tuple& point, const IntersectionParams& /*intersection*/) const
{
	float px = abs(point.X());
	float py = abs(point.Y());
	float pz = abs(point.Z());

	float maxc = std::max({ px, py, pz });

	if (maxc == px)
	{
		return Tuple::Point(point.X(), 0, 0);
	}
	else if (maxc == py)
	{
		return Tuple::Point(0, point.Y(), 0);
	}
	else
	{
		return Tuple::Point(0, 0, point.Z());
	}
}

IntersectionParams Cube::LocalIntersect(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
{
	float tmin;
	float tmax;
	if (getBounds().Intersects(ray, tmin, tmax))
	{
		intersectionDistances.emplace_back(std::make_pair(tmin, shared_from_this()));
		intersectionDistances.emplace_back(std::make_pair(tmax, shared_from_this()));

		return IntersectionParams(tmin);
	}
	else
	{
		return IntersectionParams();
	}
}