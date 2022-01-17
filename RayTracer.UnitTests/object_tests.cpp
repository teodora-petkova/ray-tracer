#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#include "source\object.h"
#include "source\transformations.h"
#include "customobject.h"
TEST(ObjectTests, Intersecting_a_scaled_shape_with_a_ray)
{
	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));

	ObjectPtr o = std::make_shared<CustomObject>(std::make_shared<Material>(),
		Transformations::Scaling(2, 2, 2));

	IntersectionInfo intersection = o->Intersect(ray, std::vector<std::pair<float, ObjectConstPtr>>());

	Ray transformedRay = std::static_pointer_cast<CustomObject>(o)->getTransformedRay();

	EXPECT_EQ(transformedRay.getOrigin(), Tuple::Point(0, 0, -2.5));
	EXPECT_EQ(transformedRay.getDirection(), Tuple::Vector(0, 0, 0.5));
}

TEST(ObjectTests, Intersecting_a_translated_shape_with_a_ray)
{
	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));

	ObjectPtr o = std::make_shared<CustomObject>(std::make_shared<Material>(),
		Transformations::Translation(5, 0, 0));

	IntersectionInfo intersection = o->Intersect(ray,
		std::vector<std::pair<float, ObjectConstPtr>>());

	Ray transformedRay = std::static_pointer_cast<CustomObject>(o)->getTransformedRay();

	EXPECT_EQ(transformedRay.getOrigin(), Tuple::Point(-5, 0, -5));
	EXPECT_EQ(transformedRay.getDirection(), Tuple::Vector(0, 0, 1));
}

TEST(ObjectTests, Computing_the_normal_on_a_translated_shape)
{
	CustomObject o = CustomObject(
		std::make_shared<Material>(),
		Transformations::Translation(0, 1, 0));

	Tuple normal = o.getNormal(Tuple::Point(0, 1.70711f, -0.70711f));

	EXPECT_EQ(normal, Tuple::Vector(0, 0.70711f, -0.70711f));
}

TEST(ObjectTests, Computing_the_normal_on_a_transformed_shape)
{
	CustomObject o = CustomObject(std::make_shared<Material>(),
		Transformations::Scaling(1, 0.5, 1) *
		Transformations::RotationZ(float(80 / 5)));

	float sqrt2over2 = sqrtf(2) / 2;
	Tuple normal = o.getNormal(Tuple::Point(0, sqrt2over2, -sqrt2over2));

	EXPECT_EQ(normal, Tuple::Vector(0, 0.97014f, -0.24254f));
}
