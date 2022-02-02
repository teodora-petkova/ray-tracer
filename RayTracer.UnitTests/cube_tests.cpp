#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#define _USE_MATH_DEFINES

#include <math.h>
#include "source\cube.h"
#include "source\transformations.h"

struct RayCubeIntersection
{
	Ray ray;
	float t1;
	float t2;

	friend std::ostream& operator<<(std::ostream& os, const RayCubeIntersection& obj)
	{
		return os
			<< "ray: " << obj.ray
			<< " t1: " << obj.t1
			<< " t2: " << obj.t2;
	}
};

class RayIntersectsCubeTests : public testing::TestWithParam<RayCubeIntersection> {};

TEST_P(RayIntersectsCubeTests, A_ray_intersects_a_cube)
{
	auto& rayCubeIntersection = GetParam();

	ObjectPtr cube = std::make_shared<Cube>();

	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>();
	IntersectionInfo intersection = cube->Intersect(rayCubeIntersection.ray, intersections);

	//EXPECT_EQ(intersection.getIsHit(), true);
	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections[0].first, rayCubeIntersection.t1);
	EXPECT_EQ(intersections[1].first, rayCubeIntersection.t2);
}

INSTANTIATE_TEST_CASE_P(CubeTests, RayIntersectsCubeTests,
	testing::Values(
		RayCubeIntersection{ Ray(Tuple::Point(5, 0.5, 0), Tuple::Vector(-1, 0, 0)), 4, 6 },
		RayCubeIntersection{ Ray(Tuple::Point(-5, 0.5, 0), Tuple::Vector(1, 0, 0)), 4, 6 },
		RayCubeIntersection{ Ray(Tuple::Point(0.5, 5, 0), Tuple::Vector(0, -1, 0)), 4, 6 },
		RayCubeIntersection{ Ray(Tuple::Point(0.5, -5, 0), Tuple::Vector(0, 1, 0)), 4, 6 },
		RayCubeIntersection{ Ray(Tuple::Point(0.5, 0, 5), Tuple::Vector(0, 0, -1)), 4, 6 },
		RayCubeIntersection{ Ray(Tuple::Point(0.5, 0, -5), Tuple::Vector(0, 0, 1)), 4, 6 },
		RayCubeIntersection{ Ray(Tuple::Point(0, 0.5, 0), Tuple::Vector(0, 0, 1)), -1, 1 }));

class RayMissesCubeTests : public testing::TestWithParam<RayCubeIntersection> {};

TEST_P(RayMissesCubeTests, A_ray_misses_a_cube)
{
	auto& rayCubeIntersection = GetParam();

	ObjectPtr cube = std::make_shared<Cube>();

	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>();
	IntersectionInfo intersection = cube->Intersect(rayCubeIntersection.ray, intersections);

	EXPECT_EQ(intersections.size(), 0);
}

INSTANTIATE_TEST_CASE_P(CubeTests, RayMissesCubeTests,
	testing::Values(
		RayCubeIntersection{ Ray(Tuple::Point(-2, 0, 0), Tuple::Vector(0.2673, 0.5345, 0.8018)), 0, 0 },
		RayCubeIntersection{ Ray(Tuple::Point(0, -2, 0), Tuple::Vector(0.8018, 0.2673, 0.5345)), 0, 0 },
		RayCubeIntersection{ Ray(Tuple::Point(0, 0, -2), Tuple::Vector(0.5345, 0.8018, 0.2673)), 0, 0 },
		RayCubeIntersection{ Ray(Tuple::Point(1.01, 0, 1.01), Tuple::Vector(0, 0, -1)), 0, 0 },
		RayCubeIntersection{ Ray(Tuple::Point(0, 1.01, 1.01), Tuple::Vector(0, -1, 0)), 0, 0 },
		RayCubeIntersection{ Ray(Tuple::Point(1.01, 1.01, 0), Tuple::Vector(-1, 0, 0)), 0, 0 }));

struct CubeNormalData
{
	Tuple point;
	Tuple normal;

	friend std::ostream& operator<<(std::ostream& os, const CubeNormalData& obj)
	{
		return os
			<< "point: " << obj.point
			<< " normal: " << obj.normal;
	}
};

class CubeNormalTests : public testing::TestWithParam<CubeNormalData> {};

TEST_P(CubeNormalTests, The_normal_on_the_surface_of_a_cube)
{
	auto& cubeNormalData = GetParam();

	ObjectPtr cube = std::make_shared<Cube>();

	auto normal = cube->getNormal(cubeNormalData.point);

	EXPECT_EQ(normal, cubeNormalData.normal);
}

INSTANTIATE_TEST_CASE_P(CubeTests, CubeNormalTests,
	testing::Values(
		CubeNormalData{ Tuple::Point(1, 0.5, -0.8), Tuple::Vector(1, 0, 0) },
		CubeNormalData{ Tuple::Point(-1, -0.2, 0.9), Tuple::Vector(-1, 0, 0) },
		CubeNormalData{ Tuple::Point(-0.4, 1, -0.1), Tuple::Vector(0, 1, 0) },
		CubeNormalData{ Tuple::Point(0.3, -1, -0.7), Tuple::Vector(0, -1, 0) },
		CubeNormalData{ Tuple::Point(-0.6, 0.3, 1), Tuple::Vector(0, 0, 1) },
		CubeNormalData{ Tuple::Point(0.4, 0.4, -1), Tuple::Vector(0, 0, -1) },
		CubeNormalData{ Tuple::Point(1, 1, 1), Tuple::Vector(1, 0, 0) },
		CubeNormalData{ Tuple::Point(-1, -1, -1), Tuple::Vector(-1, 0, 0) }));

TEST(CubeTests, A_cube_has_a_bounding_box)
{
	ObjectPtr cube = std::make_shared<Cube>();

	EXPECT_EQ(cube->getBounds().getMin(), Tuple::Point(-1, -1, -1));
	EXPECT_EQ(cube->getBounds().getMax(), Tuple::Point(1, 1, 1));
}

TEST(CubeTests, A_cube_has_a_transformed_bounding_box)
{
	Matrix<4, 4> matrix = Transformations::Translation(1, 1, 1) *
		Transformations::Scaling(2, 2, 2) *
		Transformations::RotationX(45) *
		Transformations::RotationY(45);

	ObjectPtr cube = std::make_shared<Cube>(std::make_shared<Material>(), matrix);

	auto bb = cube->getBoundsInParentSpace();

	ASSERT_EQ(bb.getMin(), Tuple::Point(-1.82843, -2.41421, -2.41421));
	ASSERT_EQ(bb.getMax(), Tuple::Point(3.82843, 4.41421, 4.41421));
}

TEST(CubeTests, A_ray_intersects_a_transformed_cube)
{
	Matrix<4, 4> matrix = Transformations::Translation(1, 0, 0);
	ObjectPtr cube = std::make_shared<Cube>(std::make_shared<Material>(), matrix);

	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));
	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>();
	IntersectionInfo intersection = cube->Intersect(ray, intersections);

	ASSERT_EQ(intersection.getIsHit(), true);
	ASSERT_EQ(intersection.getDistance(), 4);
	ASSERT_EQ(intersection.getNormal(), Tuple::Vector(-1, 0, 0));
	ASSERT_EQ(intersections[0].first, 4);
	ASSERT_EQ(intersections[1].first, 6);
}