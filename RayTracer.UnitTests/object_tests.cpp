#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#include "source\object.h"
#include "source\transformations.h"

class CustomObject :public Object
{
public:
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
	std::pair<bool, float> LocalIntersect(const Ray& ray) const
	{
		const_cast<CustomObject*>(this)->transformedRay = ray;
		return std::make_pair(false, 0.0f);
	}

	Tuple getLocalNormal(const Tuple& point) const
	{
		return Tuple::Vector(point.X(), point.Y(), point.Z());
	}

	Ray transformedRay;
};

TEST(ObjectTests, Intersecting_a_scaled_shape_with_a_ray)
{
	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));

	CustomObject o = CustomObject(std::make_shared<Material>(Material()),
		Transformations::Scaling(2, 2, 2));

	IntersectionInfo intersection = o.Intersect(ray);

	Ray transformedRay = o.getTransformedRay();

	EXPECT_EQ(transformedRay.getOrigin(), Tuple::Point(0, 0, -2.5));
	EXPECT_EQ(transformedRay.getDirection(), Tuple::Vector(0, 0, 0.5));
}


TEST(ObjectTests, Intersecting_a_translated_shape_with_a_ray)
{
	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));

	CustomObject o = CustomObject(std::make_shared<Material>(Material()),
		Transformations::Translation(5, 0, 0));

	IntersectionInfo intersection = o.Intersect(ray);

	Ray transformedRay = o.getTransformedRay();

	EXPECT_EQ(transformedRay.getOrigin(), Tuple::Point(-5, 0, -5));
	EXPECT_EQ(transformedRay.getDirection(), Tuple::Vector(0, 0, 1));
}

TEST(ObjectTests, Computing_the_normal_on_a_translated_shape)
{
	CustomObject o = CustomObject(
		std::make_shared<Material>(Material()),
		Transformations::Translation(0, 1, 0));

	Tuple normal = o.getNormal(Tuple::Point(0, 1.70711, -0.70711));

	EXPECT_EQ(normal, Tuple::Vector(0, 0.70711, -0.70711));
}

TEST(ObjectTests, Computing_the_normal_on_a_transformed_shape)
{
	CustomObject o = CustomObject(std::make_shared<Material>(Material()),
		Transformations::Scaling(1, 0.5, 1) *
		Transformations::RotationZ(float(80 / 5)));

	float sqrt2over2 = sqrt(2) / 2;
	Tuple normal = o.getNormal(Tuple::Point(0, sqrt2over2, -sqrt2over2));

	EXPECT_EQ(normal, Tuple::Vector(0, 0.97014, -0.24254));
}
