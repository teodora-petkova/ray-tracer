#pragma once
#pragma warning(push, 0)
#include <tuple>
#include <set>
#pragma warning(pop)

#include "tuple.h"
#include "ray.h"
#include "object.h"

class RAYTRACER_EXPORT Group : public virtual Object
{
public:

	Group(std::string name = "") :
		Object(name)
	{ }

	Group(const MaterialPtr& material, const Matrix<4, 4>& transformation,
		const std::string& name = "") :
		Object(material, transformation, name)
	{ }

	void AddChild(const ObjectPtr& object);

	void AddSubGroup(std::vector<ObjectPtr> objects);

	ObjectPtr getChild(int index) const { return this->children[index]; }

	int getChildrenCount() const { return this->children.size(); }

	bool Includes(const ObjectPtr& object) const;

	std::pair<GroupPtr, GroupPtr> GetPartitionedChildren();

	void Divide(int threshold);

	bool operator==(const Group& other) const;

private:
	std::vector<std::shared_ptr<Object>> children;

	IntersectionParams LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const override;

	Tuple getLocalNormal(const Tuple& point, const IntersectionParams& intersection) const override;
};