#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)
#include "source\triangle.h"
#include "source\smoothtriangle.h"

class TriangleTests : public ::testing::Test
{
protected:
	MaterialPtr material;
	Matrix<4, 4> transformation;
	TriangleTests() {
	}

	virtual ~TriangleTests() {
	}

protected:
	void SetUp() override {
		this->material = std::make_shared<Material>();
		this->transformation = Matrix<4, 4>::IdentityMatrix();
	}
};

TEST_F(TriangleTests, Constructing_a_triangle) {
	Triangle t = Triangle(
		Tuple::Point(0, 1, 0),  // A
		Tuple::Point(-1, 0, 0), // B
		Tuple::Point(1, 0, 0),  // C
		this->material,
		this->transformation);

	EXPECT_EQ(t.getAB(), Tuple::Vector(-1, -1, 0));
	EXPECT_EQ(t.getAC(), Tuple::Vector(1, -1, 0));
	// TODO: to check for the triangle normal?
	// right-handed coordinate system?
	EXPECT_EQ(t.getTriangleNormal(), Tuple::Vector(0, 0, 1));
}

TEST_F(TriangleTests, The_ray_is_parallel_to_the_triangle) {
	ObjectPtr t = std::make_shared<Triangle>(
		Tuple::Point(0, 1, 0),
		Tuple::Point(-1, 0, 0),
		Tuple::Point(1, 0, 0),
		this->material,
		this->transformation);
	Ray ray = Ray(Tuple::Point(0, -1, -2), Tuple::Vector(0, 1, 0));
	IntersectionInfo intersection = t->Intersect(ray,
		std::vector<std::pair<float, ObjectConstPtr>>());
	EXPECT_EQ(intersection.getIsHit(), false);
}

TEST_F(TriangleTests, The_ray_misses_AC_traingle_edge) {
	ObjectPtr t = std::make_shared<Triangle>(
		Tuple::Point(0, 1, 0),
		Tuple::Point(-1, 0, 0),
		Tuple::Point(1, 0, 0),
		this->material,
		this->transformation);
	Ray ray = Ray(Tuple::Point(1, 1, -2), Tuple::Vector(0, 0, 1));
	IntersectionInfo intersection = t->Intersect(ray,
		std::vector<std::pair<float, ObjectConstPtr>>());
	EXPECT_EQ(intersection.getIsHit(), false);
}

TEST_F(TriangleTests, The_ray_misses_AB_traingle_edge) {
	ObjectPtr t = std::make_shared<Triangle>(
		Tuple::Point(0, 1, 0),
		Tuple::Point(-1, 0, 0),
		Tuple::Point(1, 0, 0),
		this->material,
		this->transformation);
	Ray ray = Ray(Tuple::Point(-1, 1, -2), Tuple::Vector(0, 0, 1));
	IntersectionInfo intersection = t->Intersect(ray,
		std::vector<std::pair<float, ObjectConstPtr>>());
	EXPECT_EQ(intersection.getIsHit(), false);
}

TEST_F(TriangleTests, The_ray_misses_BC_traingle_edge) {
	ObjectPtr t = std::make_shared<Triangle>(
		Tuple::Point(0, 1, 0),
		Tuple::Point(-1, 0, 0),
		Tuple::Point(1, 0, 0),
		this->material,
		this->transformation);
	Ray ray = Ray(Tuple::Point(0, -1, -2), Tuple::Vector(0, 0, 1));
	IntersectionInfo intersection = t->Intersect(ray,
		std::vector<std::pair<float, ObjectConstPtr>>());
	EXPECT_EQ(intersection.getIsHit(), false);
}

TEST_F(TriangleTests, The_ray_intersects_the_triangle) {
	ObjectPtr t = std::make_shared<Triangle>(
		Tuple::Point(0, 1, 0),
		Tuple::Point(-1, 0, 0),
		Tuple::Point(1, 0, 0),
		this->material,
		this->transformation);
	Ray ray = Ray(Tuple::Point(0, 0.5, -2), Tuple::Vector(0, 0, 1));
	IntersectionInfo intersection = t->Intersect(ray,
		std::vector<std::pair<float, ObjectConstPtr>>());
	EXPECT_EQ(intersection.getIsHit(), true);
	EXPECT_EQ(intersection.getIntersectionPoint(), Tuple::Point(0, 0.5, 0));
	EXPECT_EQ(intersection.getDistance(), 2);
	EXPECT_EQ(intersection.getNormal(), Tuple::Vector(0, 0, -1));
}

TEST_F(TriangleTests, Preparing_the_normal_on_a_smooth_triangle)
{
Background:
	ObjectPtr t = std::make_shared<SmoothTriangle>(
		Tuple::Point(0, 1, 0),
		Tuple::Point(-1, 0, 0),
		Tuple::Point(1, 0, 0),
		Tuple::Vector(0, 1, 0),
		Tuple::Vector(-1, 0, 0),
		Tuple::Vector(1, 0, 0));
	Ray ray = Ray(Tuple::Point(-0.2, 0.3, -2), Tuple::Vector(0, 0, 1));

	auto intersections = std::vector<std::pair<float, ObjectConstPtr>>();
	auto intersection = t->Intersect(ray, intersections);

	EXPECT_EQ(intersection.getIsHit(), true);
	EXPECT_EQ(intersection.getIntersectionPoint(), Tuple::Point(-0.2, 0.3, 0));
	EXPECT_EQ(intersection.getDistance(), 2);
	EXPECT_EQ(intersection.getNormal(), Tuple::Vector(-0.5547, 0.83205, 0));
}