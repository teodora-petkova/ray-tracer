#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#define _USE_MATH_DEFINES

#include <math.h>
#include "source\plane.h"

TEST(PlaneTests, The_normal_of_a_plane_is_constant_everywhere) {
	ObjectPtr p = std::make_shared<Plane>();

	Tuple n1 = p->getNormal(Tuple::Point(0, 0, 0));
	Tuple n2 = p->getNormal(Tuple::Point(10, 0, -10));
	Tuple n3 = p->getNormal(Tuple::Point(-5, 0, 150));

	EXPECT_EQ(n1, Tuple::Vector(0, 1, 0));
	EXPECT_EQ(n2, Tuple::Vector(0, 1, 0));
	EXPECT_EQ(n3, Tuple::Vector(0, 1, 0));
}

TEST(PlaneTests, Intersect_with_a_ray_parallel_to_the_plane) {
	ObjectPtr p = std::make_shared<Plane>();

	Ray r = Ray(Tuple::Point(0, 10, 0), Tuple::Vector(0, 0, 1));

	IntersectionInfo info = p->Intersect(r,
		std::vector<std::pair<float, ObjectConstPtr>>());

	EXPECT_EQ(info.getIsHit(), false);
}

TEST(PlaneTests, Intersect_with_a_coplanar_ray) {
	ObjectPtr p = std::make_shared<Plane>();

	Ray r = Ray(Tuple::Point(0, 0, 0), Tuple::Vector(0, 0, 1));

	IntersectionInfo info = p->Intersect(r,
		std::vector<std::pair<float, ObjectConstPtr>>());

	EXPECT_EQ(info.getIsHit(), false);
}

TEST(PlaneTests, A_ray_intersecting_a_plane_from_above) {
	ObjectPtr p = std::make_shared<Plane>();

	Ray r = Ray(Tuple::Point(0, 1, 0), Tuple::Vector(0, -1, 0));

	IntersectionInfo info = p->Intersect(r,
		std::vector<std::pair<float, ObjectConstPtr>>());

	EXPECT_EQ(info.getIsHit(), true);
	EXPECT_EQ(info.getDistance(), 1);
	EXPECT_EQ(info.getIntersectionPoint(), Tuple::Point(0, 0, 0));
	EXPECT_EQ(info.getNormal(), Tuple::Vector(0, 1, 0));
}

TEST(PlaneTests, A_ray_intersecting_a_plane_from_below) {
	ObjectPtr p = std::make_shared<Plane>();

	Ray r = Ray(Tuple::Point(0, -1, 0), Tuple::Vector(0, 1, 0));

	IntersectionInfo info = p->Intersect(r, std::vector<std::pair<float, ObjectConstPtr>>());

	EXPECT_EQ(info.getIsHit(), true);
	EXPECT_EQ(info.getDistance(), 1);
	EXPECT_EQ(info.getIntersectionPoint(), Tuple::Point(0, 0, 0));
	EXPECT_EQ(info.getNormal(), Tuple::Vector(0, -1, 0));
}

TEST(PlaneTests, A_plane_has_a_bounding_box) {
	ObjectPtr plane = std::make_shared<Plane>();

	EXPECT_EQ(plane->getBounds().getMin(), Tuple::Vector(-INFINITY, 0, -INFINITY));
	EXPECT_EQ(plane->getBounds().getMax(), Tuple::Vector(INFINITY, 0, INFINITY));
}