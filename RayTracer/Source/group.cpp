#include "group.h"

void Group::AddChild(const ObjectPtr& object)
{
	object->setParent(shared_from_this());
	object->setMaterial(this->material);

	this->bounds.AddBound(object->getBoundsInParentSpace());
	this->boundsInParentSpace = this->bounds.Transform(this->transformation);

	children.push_back(object);
}

bool Group::Includes(const ObjectPtr& object) const
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

void Group::AddSubGroup(std::vector<ObjectPtr> objects)
{
	GroupPtr subGroup = std::make_shared<Group>();
	for (ObjectPtr obj : objects)
	{
		subGroup->AddChild(obj);
	}
	this->AddChild(subGroup);
}

std::pair<GroupPtr, GroupPtr> Group::GetPartitionedChildren()
{
	BoundingBox leftBB, rightBB;
	std::tie(leftBB, rightBB) = this->getBounds().Split();

	GroupPtr leftGroup = std::make_shared<Group>();
	GroupPtr rightGroup = std::make_shared<Group>();

	std::set<int> itemsToRemove;
	int i = 0;
	for (ObjectPtr child : this->children)
	{
		if (leftBB.Contains(child->getBoundsInParentSpace()))
		{
			leftGroup->AddChild(child);
			itemsToRemove.insert(i);
		}
		else if (rightBB.Contains(child->getBoundsInParentSpace()))
		{
			rightGroup->AddChild(child);
			itemsToRemove.insert(i);
		}
		i++;
	}

	i = 0;
	this->children.erase(
		std::remove_if(
			this->children.begin(),
			this->children.end(),
			[itemsToRemove, &i](ObjectPtr obj) {
				i++;
				return itemsToRemove.count(i - 1) == 1; }
		),
		this->children.end());

	return std::make_pair(leftGroup, rightGroup);
}

void Group::Divide(int threshold)
{
	if (threshold <= 0)
	{
		// no partition
		return;
	}

	int currentChildrenCount = this->getChildrenCount();
	if (threshold <= currentChildrenCount)
	{
		GroupPtr left, right;
		std::tie(left, right) = this->GetPartitionedChildren();
		if (currentChildrenCount == left->getChildrenCount()
			&& right->getChildrenCount() == 0)
		{
			// no new partition is present
			*this = *left;
		}
		else if (currentChildrenCount == right->getChildrenCount()
			&& left->getChildrenCount() == 0)
		{
			// no new partition is present
			*this = *right;
		}
		else
		{
			if (left->getChildrenCount() > 0)
			{
				this->AddChild(left);
			}
			if (right->getChildrenCount() > 0)
			{
				this->AddChild(right);
			}
		}
	}

	for (ObjectPtr child : this->children)
	{
		child->Divide(threshold);
	}
}

bool Group::operator==(const Group& other) const
{
	bool areEqual = true;
	if (this->children.size() == other.children.size())
	{
		for (int i = 0; i < this->children.size(); i++)
		{
			// in order to call operator== for specific object types!
			areEqual = *this->children[i] == *other.children[i];
		}
	}
	else
	{
		areEqual = false;
	}
	return areEqual && Object::operator==(other);
}