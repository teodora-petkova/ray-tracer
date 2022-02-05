#include "source\object.h"

class CustomObject :public Object
{
public:
	CustomObject()
		:Object()
	{
		this->transformedRay = nullptr;
		this->bounds = BoundingBox(Tuple::Point(-1, -1, -1), Tuple::Point(1, 1, 1));
		this->boundsInParentSpace = this->bounds.Transform(transformation);
	}

	CustomObject(MaterialPtr material,
		Matrix<4, 4> transformation)
		: Object(material, transformation)
	{
		this->transformedRay = nullptr;
		this->bounds = BoundingBox(Tuple::Point(-1, -1, -1), Tuple::Point(1, 1, 1));
		this->boundsInParentSpace = this->bounds.Transform(transformation);
	}

	std::shared_ptr<Ray> getTransformedRay() const
	{
		return this->transformedRay;
	}

	void Divide(int threshold) {}
private:
	IntersectionParams LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
	{
		const_cast<CustomObject*>(this)->transformedRay = std::make_shared<Ray>(ray);
		return IntersectionParams();
	}

	Tuple getLocalNormal(const Tuple& point, const IntersectionParams& /*intersection*/) const
	{
		return Tuple::Vector(point.X(), point.Y(), point.Z());
	}

	std::shared_ptr<Ray> transformedRay;
};
