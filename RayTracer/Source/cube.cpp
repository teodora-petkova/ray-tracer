#include "cube.h"

Tuple Cube::getLocalNormal(const Tuple& point) const
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

std::pair<float, float> getIntersectionDistancesAtAxis(float rayOrigin, float rayDirection)
{
	//float tmin = (-1 - rayOrigin) * INFINITY;
	//float tmax = (1 - rayOrigin) * INFINITY;

	//if (!isCloseToZero(rayDirection))
	//{
	float const oneOverDirection = 1.f / rayDirection;

	float tmin = (1 - rayOrigin) * oneOverDirection;
	float tmax = (-1 - rayOrigin) * oneOverDirection;
	//}

	if (tmin > tmax)
	{
		std::swap(tmin, tmax);
	}

	return std::make_pair(tmin, tmax);
}

float Cube::LocalIntersect(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
{
	auto xt = getIntersectionDistancesAtAxis(ray.getOrigin().X(), ray.getDirection().X());
	auto yt = getIntersectionDistancesAtAxis(ray.getOrigin().Y(), ray.getDirection().Y());
	auto zt = getIntersectionDistancesAtAxis(ray.getOrigin().Z(), ray.getDirection().Z());

	float tmin = std::max({ xt.first, yt.first, zt.first });
	float tmax = std::min({ xt.second, yt.second, zt.second });

	if (tmin < tmax)
	{
		intersectionDistances.emplace_back(std::make_pair(tmin, shared_from_this()));
		intersectionDistances.emplace_back(std::make_pair(tmax, shared_from_this()));

		return tmin;
	}
	else
	{
		return INFINITY;
	}
}