#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)
#include "source\ray.h"
#include "source\transformations.h"

TEST(RayTests, TranslateARay) {
	Ray ray = Ray(Tuple::Point(1, 2, 3), Tuple::Vector(0, 1, 0));
	Matrix<4, 4> m = Transformations::Translation(3, 4, 5);
	Ray transformedRay = ray * m;

	EXPECT_EQ(transformedRay.getOrigin(), Tuple::Point(4, 6, 8));
	EXPECT_EQ(transformedRay.getDirection(), Tuple::Point(0, 1, 0));
}

TEST(RayTests, ScaleARay) {
	Ray ray = Ray(Tuple::Point(1, 2, 3), Tuple::Vector(0, 1, 0));
	Matrix<4, 4> m = Transformations::Scaling(2, 3, 4);
	Ray transformedRay = ray * m;

	EXPECT_EQ(transformedRay.getOrigin(), Tuple::Point(2, 6, 12));
	EXPECT_EQ(transformedRay.getDirection(), Tuple::Point(0, 3, 0));
}