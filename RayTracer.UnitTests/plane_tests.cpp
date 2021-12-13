#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#define _USE_MATH_DEFINES

#include <math.h>
#include "source\plane.h"

TEST(PlaneTests, The_normal_of_a_plane_is_constant_everywhere) {
	Plane p = Plane();

	Tuple n1 = p.getNormal(Tuple::Point(0, 0, 0));
	Tuple n2 = p.getNormal(Tuple::Point(10, 0, -10));
	Tuple n3 = p.getNormal(Tuple::Point(-5, 0, 150));

	EXPECT_EQ(n1, Tuple::Vector(0, 1, 0));
	EXPECT_EQ(n2, Tuple::Vector(0, 1, 0));
	EXPECT_EQ(n3, Tuple::Vector(0, 1, 0));
}

TEST(PlaneTests, Intersect_with_a_ray_parallel_to_the_plane) {
	Plane p = Plane();

	Ray r = Ray(Tuple::Point(0, 10, 0), Tuple::Vector(0, 0, 1));

	IntersectionInfo info = p.Intersect(r);

	EXPECT_EQ(info.getIsHit(), false);
}

TEST(PlaneTests, Intersect_with_a_coplanar_ray) {
	Plane p = Plane();

	Ray r = Ray(Tuple::Point(0, 0, 0), Tuple::Vector(0, 0, 1));

	IntersectionInfo info = p.Intersect(r);

	EXPECT_EQ(info.getIsHit(), false);
}

TEST(PlaneTests, A_ray_intersecting_a_plane_from_above) {
	Plane p = Plane();

	Ray r = Ray(Tuple::Point(0, 1, 0), Tuple::Vector(0, -1, 0));

	IntersectionInfo info = p.Intersect(r);

	EXPECT_EQ(info.getIsHit(), true);
	EXPECT_EQ(info.getDistance(), 1);
	EXPECT_EQ(info.getIntersectionPoint(), Tuple::Point(0, 0, 0));
	EXPECT_EQ(info.getNormal(), Tuple::Vector(0, 1, 0));
}

TEST(PlaneTests, A_ray_intersecting_a_plane_from_below) {
	Plane p = Plane();

	Ray r = Ray(Tuple::Point(0, -1, 0), Tuple::Vector(0, 1, 0));

	IntersectionInfo info = p.Intersect(r);

	EXPECT_EQ(info.getIsHit(), true);
	EXPECT_EQ(info.getDistance(), 1);
	EXPECT_EQ(info.getIntersectionPoint(), Tuple::Point(0, 0, 0));
	EXPECT_EQ(info.getNormal(), Tuple::Vector(0, 1, 0));
}