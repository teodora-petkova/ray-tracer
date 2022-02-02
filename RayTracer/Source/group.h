#pragma once

#include "tuple.h"
#include "ray.h"
#include "object.h"

class RAYTRACER_EXPORT Group : public virtual Object
{
public:

	Group(std::string name = "") :
		Object(),
		name(name)
	{ }

	Group(MaterialPtr material, Matrix<4, 4> transformation, std::string name = "") :
		Object(material, transformation),
		name(name)
	{ }

	void AddChild(ObjectPtr object);

	ObjectPtr getChild(int index) { return this->children[index]; }

	bool Includes(ObjectPtr object) const;

private:
	std::vector<std::shared_ptr<Object>> children;
	std::string name;

	IntersectionParams LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const override;

	Tuple getLocalNormal(const Tuple& point, const IntersectionParams& intersection) const override;
};