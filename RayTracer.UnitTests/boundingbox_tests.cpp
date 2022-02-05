#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#include "source\boundingbox.h"
#include "source\transformations.h"

TEST(BoundingBoxesTests, Creating_an_empty_bounding_box)
{
	BoundingBox bb = BoundingBox();
	ASSERT_EQ(bb.getMin(), Tuple::Point(INFINITY, INFINITY, INFINITY));
	ASSERT_EQ(bb.getMax(), Tuple::Point(-INFINITY, -INFINITY, -INFINITY));
}

TEST(BoundingBoxesTests, Creating_a_bounding_box_with_volume)
{
	BoundingBox bb = BoundingBox(Tuple::Point(-1, -2, -3), Tuple::Point(3, 2, 1));
	ASSERT_EQ(bb.getMin(), Tuple::Point(-1, -2, -3));
	ASSERT_EQ(bb.getMax(), Tuple::Point(3, 2, 1));
}

TEST(BoundingBoxesTests, Adding_points_to_an_empty_bounding_box)
{
	BoundingBox bb = BoundingBox();
	bb.AddPoint(Tuple::Point(-5, 2, 0));
	bb.AddPoint(Tuple::Point(7, 0, -3));
	ASSERT_EQ(bb.getMin(), Tuple::Point(-5, 0, -3));
	ASSERT_EQ(bb.getMax(), Tuple::Point(7, 2, 0));
}

TEST(BoundingBoxesTests, Adding_one_bouding_box_to_another)
{
	BoundingBox bb1 = BoundingBox(Tuple::Point(-5, -2, 0), Tuple::Point(7, 4, 4));
	BoundingBox bb2 = BoundingBox(Tuple::Point(8, -7, -2), Tuple::Point(14, 2, 8));

	bb1.AddBound(bb2);
	ASSERT_EQ(bb1.getMin(), Tuple::Point(-5, -7, -2));
	ASSERT_EQ(bb1.getMax(), Tuple::Point(14, 4, 8));
}

struct PointData
{
	Tuple point;
	bool isInBounds;

	friend std::ostream& operator<<(std::ostream& os, const PointData& p)
	{
		return os
			<< "point: " << p.point
			<< " is in bounds: " << (p.isInBounds ? "true" : "false");
	}
};

class BBoxesContainingPointsTests : public testing::TestWithParam<PointData> {};

TEST_P(BBoxesContainingPointsTests, Checking_to_see_if_a_box_contains_a_given_point)
{
	BoundingBox bb = BoundingBox(Tuple::Point(5, -2, 0), Tuple::Point(11, 4, 7));

	auto& pointData = GetParam();
	ASSERT_EQ(bb.Contains(pointData.point), pointData.isInBounds);
}

INSTANTIATE_TEST_CASE_P(BoundingBoxesTests, BBoxesContainingPointsTests,
	testing::Values(
		PointData{ Tuple::Point(5, -2, 0), true },
		PointData{ Tuple::Point(11, 4, 7), true },
		PointData{ Tuple::Point(8, 1, 3), true },
		PointData{ Tuple::Point(3, 0, 3), false },
		PointData{ Tuple::Point(8, -4, 3), false },
		PointData{ Tuple::Point(8, 1, -1), false },
		PointData{ Tuple::Point(13, 1, 3), false },
		PointData{ Tuple::Point(8, 5, 3), false },
		PointData{ Tuple::Point(8, 1, 8), false }));


struct BBData
{
	BoundingBox bound;
	bool isInBounds;

	friend std::ostream& operator<<(std::ostream& os, const BBData& b)
	{
		return os
			<< "point: " << b.bound
			<< " is in bounds: " << (b.isInBounds ? "true" : "false");
	}
};

class BBoxesContainingBBoxesTests : public testing::TestWithParam<BBData> {};

TEST_P(BBoxesContainingBBoxesTests, Checking_to_see_if_a_box_contains_a_given_box)
{
	BoundingBox bb = BoundingBox(Tuple::Point(5, -2, 0), Tuple::Point(11, 4, 7));

	auto& bbData = GetParam();
	ASSERT_EQ(bb.Contains(bbData.bound), bbData.isInBounds);
}

INSTANTIATE_TEST_CASE_P(BoundingBoxesTests, BBoxesContainingBBoxesTests,
	testing::Values(
		BBData{ BoundingBox(Tuple::Point(5, -2, 0), Tuple::Point(11, 4, 7)), true },
		BBData{ BoundingBox(Tuple::Point(6, -1, 1), Tuple::Point(10, 3, 6)), true },
		BBData{ BoundingBox(Tuple::Point(4, -3, -1), Tuple::Point(10, 3, 6)), false },
		BBData{ BoundingBox(Tuple::Point(6, -1, 1), Tuple::Point(12, 5, 8)), false }));

TEST(BoundingBoxesTests, Transforming_a_bounding_box)
{
	BoundingBox bb1 = BoundingBox(Tuple::Point(-1, -1, -1), Tuple::Point(1, 1, 1));

	Matrix<4, 4> matrix = Transformations::Translation(1, 1, 1) *
		Transformations::Scaling(2, 2, 2) *
		Transformations::RotationX(45) *
		Transformations::RotationY(45);
	BoundingBox bb2 = bb1.Transform(matrix);

	ASSERT_EQ(bb2.getMin(), Tuple::Point(-1.82843, -2.41421, -2.41421));
	ASSERT_EQ(bb2.getMax(), Tuple::Point(3.82843, 4.41421, 4.41421));
}

struct RayBBoxIntersection
{
	Ray ray;
	bool isBBoxHit;

	friend std::ostream& operator<<(std::ostream& os, const RayBBoxIntersection& obj)
	{
		return os
			<< "ray: " << obj.ray
			<< " is BBox hit: " << (obj.isBBoxHit ? "true" : "false");
	}
};

class RayIntersectsCubicBBoxTests : public testing::TestWithParam<RayBBoxIntersection> {};

TEST_P(RayIntersectsCubicBBoxTests, Intersect)
{
	BoundingBox bb = BoundingBox(Tuple::Point(-1, -1, -1), Tuple::Point(1, 1, 1));

	auto& intersection = GetParam();
	float tmin, tmax;
	bool isHit = bb.Intersects(intersection.ray, tmin, tmax);

	ASSERT_EQ(isHit, intersection.isBBoxHit);
}

INSTANTIATE_TEST_CASE_P(BoundingBoxesTests, RayIntersectsCubicBBoxTests,
	testing::Values(

		RayBBoxIntersection{ Ray(Tuple::Point(5, 0.5, 0), Tuple::Vector(-1, 0, 0)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(-5, 0.5, 0), Tuple::Vector(1, 0, 0)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(0.5, 5, 0), Tuple::Vector(0, -1, 0)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(0.5, -5, 0), Tuple::Vector(0, 1, 0)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(0.5, 0, 5), Tuple::Vector(0, 0, -1)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(0.5, 0, -5), Tuple::Vector(0, 0, 1)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(0, 0.5, 0), Tuple::Vector(0, 0, 1)), true },

		RayBBoxIntersection{ Ray(Tuple::Point(-2, 0, 0), Tuple::Vector(2, 4, 6)), false },
		RayBBoxIntersection{ Ray(Tuple::Point(0, -2, 0), Tuple::Vector(6, 2, 4)), false },
		RayBBoxIntersection{ Ray(Tuple::Point(0, 0, -2), Tuple::Vector(4, 6, 2)), false },
		RayBBoxIntersection{ Ray(Tuple::Point(2, 0, 2), Tuple::Vector(0, 0, -1)), false },
		RayBBoxIntersection{ Ray(Tuple::Point(0, 2, 2), Tuple::Vector(0, -1, 0)), false },
		RayBBoxIntersection{ Ray(Tuple::Point(2, 2, 0), Tuple::Vector(-1, 0, 0)), false }));


class RayIntersectsNonCubicBBoxTests : public testing::TestWithParam<RayBBoxIntersection> {};

TEST_P(RayIntersectsNonCubicBBoxTests, Intersect)
{
	BoundingBox bb = BoundingBox(Tuple::Point(5, -2, 0), Tuple::Point(11, 4, 7));

	auto& intersection = GetParam();
	float tmin, tmax;
	bool isHit = bb.Intersects(intersection.ray, tmin, tmax);

	ASSERT_EQ(isHit, intersection.isBBoxHit);
}

INSTANTIATE_TEST_CASE_P(BoundingBoxesTests, RayIntersectsNonCubicBBoxTests,
	testing::Values(

		RayBBoxIntersection{ Ray(Tuple::Point(15, 1, 2), Tuple::Vector(-1, 0, 0)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(-5, -1, 4), Tuple::Vector(1, 0, 0)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(7, 6, 5), Tuple::Vector(0, -1, 0)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(9, -5, 6), Tuple::Vector(0, 1, 0)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(8, 2, 12), Tuple::Vector(0, 0, -1)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(6, 0, -5), Tuple::Vector(0, 0, 1)), true },
		RayBBoxIntersection{ Ray(Tuple::Point(8, 1, -3.5), Tuple::Vector(0, 0, 1)), true },

		RayBBoxIntersection{ Ray(Tuple::Point(9, -1, -8), Tuple::Vector(2, 4, 6)), false },
		RayBBoxIntersection{ Ray(Tuple::Point(8, 3, -4), Tuple::Vector(6, 2, 4)), false },
		RayBBoxIntersection{ Ray(Tuple::Point(9, -1, -2), Tuple::Vector(4, 6, 2)), false },
		RayBBoxIntersection{ Ray(Tuple::Point(4, 0, 9), Tuple::Vector(0, 0, -1)), false },
		RayBBoxIntersection{ Ray(Tuple::Point(8, 6, -1), Tuple::Vector(0, -1, 0)), false },
		RayBBoxIntersection{ Ray(Tuple::Point(12, 5, 4), Tuple::Vector(-1, 0, 0)), false }));

TEST(BoundingBoxesTests, Splitting_a_perfect_cube)
{
	BoundingBox bb = BoundingBox(Tuple::Point(-1, -4, -5), Tuple::Point(9, 6, 5));

	BoundingBox left, right;
	std::tie(left, right) = bb.Split();

	ASSERT_EQ(left.getMin(), Tuple::Point(-1, -4, -5));
	ASSERT_EQ(left.getMax(), Tuple::Point(4, 6, 5));

	ASSERT_EQ(right.getMin(), Tuple::Point(4, -4, -5));
	ASSERT_EQ(right.getMax(), Tuple::Point(9, 6, 5));
}

TEST(BoundingBoxesTests, Splitting_a_x_wide_cube)
{
	BoundingBox bb = BoundingBox(Tuple::Point(-1, -2, -3), Tuple::Point(9, 5.5, 3));

	BoundingBox left, right;
	std::tie(left, right) = bb.Split();

	ASSERT_EQ(left.getMin(), Tuple::Point(-1, -2, -3));
	ASSERT_EQ(left.getMax(), Tuple::Point(4, 5.5, 3));

	ASSERT_EQ(right.getMin(), Tuple::Point(4, -2, -3));
	ASSERT_EQ(right.getMax(), Tuple::Point(9, 5.5, 3));
}

TEST(BoundingBoxesTests, Splitting_a_y_wide_cube)
{
	BoundingBox bb = BoundingBox(Tuple::Point(-1, -2, -3), Tuple::Point(5, 8, 3));

	BoundingBox left, right;
	std::tie(left, right) = bb.Split();

	ASSERT_EQ(left.getMin(), Tuple::Point(-1, -2, -3));
	ASSERT_EQ(left.getMax(), Tuple::Point(5, 3, 3));

	ASSERT_EQ(right.getMin(), Tuple::Point(-1, 3, -3));
	ASSERT_EQ(right.getMax(), Tuple::Point(5, 8, 3));
}

TEST(BoundingBoxesTests, Splitting_a_z_wide_cube)
{
	BoundingBox bb = BoundingBox(Tuple::Point(-1, -2, -3), Tuple::Point(5, 3, 7));

	BoundingBox left, right;
	std::tie(left, right) = bb.Split();

	ASSERT_EQ(left.getMin(), Tuple::Point(-1, -2, -3));
	ASSERT_EQ(left.getMax(), Tuple::Point(5, 3, 2));

	ASSERT_EQ(right.getMin(), Tuple::Point(-1, -2, 2));
	ASSERT_EQ(right.getMax(), Tuple::Point(5, 3, 7));
}