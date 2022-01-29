#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#include "source\group.h"
#include "source\sphere.h"
#include "source\cube.h"
#include "source\transformations.h"
#include "customobject.h"

TEST(GroupTests, Adding_a_child_to_a_group) {
	GroupPtr group = std::make_shared<Group>();

	ObjectPtr obj = std::make_shared<CustomObject>();
	group->AddChild(obj);

	EXPECT_TRUE(group->Includes(obj));
	EXPECT_EQ(obj->getParent(), group);
}

TEST(GroupTests, Intersecting_a_ray_with_an_empty_group) {
	GroupPtr group = std::make_shared<Group>();

	Ray ray = Ray(Tuple::Point(0, 0, 0), Tuple::Vector(0, 0, 1));

	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>();
	auto intersection = group->Intersect(ray, intersections);

	EXPECT_FALSE(intersection.getIsHit());
	EXPECT_TRUE(intersections.empty());
}

TEST(GroupTests, Intersecting_a_ray_with_a_nonempty_group) {
	GroupPtr group = std::make_shared<Group>();
	ObjectPtr sphere1 = std::make_shared<Sphere>();
	ObjectPtr sphere2 = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(),
		Transformations::Translation(0, 0, -3));
	ObjectPtr sphere3 = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(),
		Transformations::Translation(5, 0, 0));

	group->AddChild(sphere1);
	group->AddChild(sphere2);
	group->AddChild(sphere3);

	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));

	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>();
	auto intersection = group->Intersect(ray, intersections);

	// sorting is done in scene.cpp when all intersections are collected!
	// TODO: to refactor?
	sort(intersections.begin(), intersections.end());

	EXPECT_EQ(intersections.size(), 4);
	EXPECT_EQ(intersections[0].second, sphere2);
	EXPECT_EQ(intersections[1].second, sphere2);
	EXPECT_EQ(intersections[2].second, sphere1);
	EXPECT_EQ(intersections[3].second, sphere1);
}

TEST(GroupTests, Intersecting_a_transformed_group) {
	GroupPtr group = std::make_shared<Group>(std::make_shared<Material>(),
		Transformations::Scaling(2, 2, 2));
	ObjectPtr sphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(),
		Transformations::Translation(5, 0, 0));
	group->AddChild(sphere);

	Ray ray = Ray(Tuple::Point(10, 0, -10), Tuple::Vector(0, 0, 1));

	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>();
	auto intersection = group->Intersect(ray, intersections);

	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections[0].first, 8);
	EXPECT_EQ(intersections[1].first, 12);
}

TEST(GroupTests, Converting_a_point_from_world_to_object_space) {
	GroupPtr group1 = std::make_shared<Group>(std::make_shared<Material>(),
		Transformations::RotationY(90));
	GroupPtr group2 = std::make_shared<Group>(std::make_shared<Material>(),
		Transformations::Scaling(2, 2, 2));
	ObjectPtr sphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(),
		Transformations::Translation(5, 0, 0));
	group2->AddChild(sphere);
	group1->AddChild(group2);

	Tuple normal = sphere->TransformFromWorldToObjectSpace(Tuple::Point(-2, 0, -10));
	EXPECT_EQ(normal, Tuple::Vector(0, 0, -1));
}

TEST(GroupTests, Converting_a_normal_from_object_to_world_space) {
	GroupPtr group1 = std::make_shared<Group>(std::make_shared<Material>(),
		Transformations::RotationY(90));
	GroupPtr group2 = std::make_shared<Group>(std::make_shared<Material>(),
		Transformations::Scaling(1, 2, 3));
	ObjectPtr sphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(),
		Transformations::Translation(5, 0, 0));
	group2->AddChild(sphere);
	group1->AddChild(group2);

	float sqrt3over3 = sqrt(3) / 3;
	Tuple normal = sphere->TransformFromObjectToWorldSpace(Tuple::Point(sqrt3over3, sqrt3over3, sqrt3over3));
	EXPECT_EQ(normal, Tuple::Vector(0.2857, 0.4286, -0.8571));
}

TEST(GroupTests, Finding_a_normal_on_a_child_object) {
	GroupPtr group1 = std::make_shared<Group>(std::make_shared<Material>(),
		Transformations::RotationY(90));
	GroupPtr group2 = std::make_shared<Group>(std::make_shared<Material>(),
		Transformations::Scaling(1, 2, 3));
	ObjectPtr sphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(),
		Transformations::Translation(5, 0, 0));
	group2->AddChild(sphere);
	group1->AddChild(group2);

	Tuple normal = sphere->getNormal(Tuple::Point(1.7321, 1.1547, -5.5774));
	EXPECT_EQ(normal, Tuple::Vector(0.2857, 0.4286, -0.8571));
}

TEST(GroupTests, A_group_has_a_bounding_box_containing_its_children)
{
	auto transform1 = Transformations::Translation(2, 5, -3) *
		Transformations::Scaling(2, 2, 2);
	ObjectPtr sphere = std::make_shared<Sphere>(Tuple::Point(0, 0, 0), 1,
		std::make_shared<Material>(), transform1);

	auto transform2 = Transformations::Scaling(1, 2, 1) *
		Transformations::Translation(-4, -1, 4) *
		Transformations::Scaling(0.5, 1, 0.5);
	ObjectPtr cube = std::make_shared<Cube>(std::make_shared<Material>(), transform2);

	GroupPtr group = std::make_shared<Group>(std::make_shared<Material>(),
		Transformations::RotationY(90));

	group->AddChild(sphere);
	group->AddChild(cube);

	// TODO: to check transformations and objects bounds afterwards again!
	EXPECT_EQ(group->getBounds().getMin(), Tuple::Point(-4.5, -4, -5));
	EXPECT_EQ(group->getBounds().getMax(), Tuple::Point(4, 7, 4.5));
}