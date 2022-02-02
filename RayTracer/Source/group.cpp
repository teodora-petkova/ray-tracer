#include "group.h"

void Group::AddChild(ObjectPtr object)
{
	object->setParent(shared_from_this());
	object->setMaterial(this->material);

	this->bounds.AddBound(object->getBoundsInParentSpace());
	this->boundsInParentSpace = this->bounds.Transform(this->transformation);

	children.push_back(object);
}

bool Group::Includes(ObjectPtr object) const
{
	for (auto& child : children)
	{
		if (child == object)
		{
			return true;
		}
	}
	return false;
}


Tuple Group::getLocalNormal(const Tuple& point, const IntersectionParams& /*intersection*/) const
{
	throw std::runtime_error("A group doesn't have a normal vector by itself. The method getNormalAtLocal() must be called directly on contained objects.");
}

IntersectionParams Group::LocalIntersect(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
{
	float tmin, tmax;
	if (this->bounds.Intersects(ray, tmin, tmax))
	{
		IntersectionParams minIntersection = IntersectionParams();
		for (auto& child : this->children)
		{
			auto intersection = child->Intersect(ray, intersectionDistances);

			// TODO: to refactor? - duplicated checks as in scene.cpp : ((
			if (intersection.getIsHit() &&
				intersection.getDistance() >= EPSILON &&
				intersection.getDistance() < minIntersection.getDistance())
			{
				minIntersection = IntersectionParams(intersection.getDistance(),
					intersection.getU(),
					intersection.getV());
			}
		}
		return minIntersection;
	}
	return IntersectionParams();
}