#include "source\object.h"

class CustomObject :public Object
{
public:
	CustomObject()
		:Object()
	{
		this->transformedRay = Ray();
	}

	CustomObject(MaterialPtr material,
		Matrix<4, 4> transformation)
		: Object(material, transformation)
	{
		this->transformedRay = Ray();
	}

	Ray getTransformedRay() const
	{
		return this->transformedRay;
	}
private:
	IntersectionParams LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
	{
		const_cast<CustomObject*>(this)->transformedRay = ray;
		return IntersectionParams();
	}

	Tuple getLocalNormal(const Tuple& point, const IntersectionParams& /*intersection*/) const
	{
		return Tuple::Vector(point.X(), point.Y(), point.Z());
	}

	Ray transformedRay;
};
