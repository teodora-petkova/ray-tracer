#pragma once

#include "tuple.h"
#include "ray.h"
#include "object.h"

class RAYTRACER_EXPORT Group : public virtual Object
{
public:

	Group(std::string name = "") : Object()
	{
		this->name = name;
	}

	Group(MaterialPtr material, Matrix<4, 4> transformation) :
		Object(material, transformation)
	{}

	void AddChild(ObjectPtr object)
	{
		children.push_back(object);
		object->setParent(shared_from_this());
	}

	ObjectPtr getChild(int index) { return this->children[index]; }

	bool Includes(ObjectPtr object) const
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

private:
	std::vector<std::shared_ptr<Object>> children;
	std::string name;

	float LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const override;

	Tuple getLocalNormal(const Tuple& ray) const override;
};