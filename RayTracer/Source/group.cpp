#include "group.h"

Tuple Group::getLocalNormal(const Tuple& point, const IntersectionParams& /*intersection*/) const
{
	/*
	// TODO: to add an exception in this case but now getNormal is called in Intersect(...) => to refactor!
	throw std::runtime_error("A group doesn't have a normal vector by itself. The method getNormalAtLocal() must be called directly on contained objects.");
	*/
	return Tuple();
}

IntersectionParams Group::LocalIntersect(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
{
	float minDistance = INFINITY;
	for (auto& child : this->children)
	{
		auto intersection = child->Intersect(ray, intersectionDistances);

		// TODO: to refactor? - duplicated checks as in scene.cpp : ((
		if (intersection.getIsHit() &&
			intersection.getDistance() >= EPSILON &&
			intersection.getDistance() < minDistance)
		{
			minDistance = intersection.getDistance();
		}
	}
	return IntersectionParams(minDistance);
}