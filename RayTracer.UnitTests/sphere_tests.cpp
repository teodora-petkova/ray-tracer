#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#define _USE_MATH_DEFINES

#include <math.h>
#include "source\sphere.h"
#include "source\transformations.h"

class SphereTests : public ::testing::Test
{
protected:
	Tuple origin;
	float radius;
	MaterialPtr material;
	Matrix<4, 4> transformation;

	SphereTests() {
	}

	virtual ~SphereTests() {
	}

protected:
	void SetUp() override {
		this->origin = Tuple::Point(0, 0, 0);
		this->radius = 1;
		this->material = std::make_shared<Material>(Material());
		this->transformation = Matrix<4, 4>::IdentityMatrix();
	}
};

TEST_F(SphereTests, The_ray_intersects_the_sphere_at_two_points) {
	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		this->transformation);

	IntersectionInfo intersection = sphere.Intersect(ray);

	EXPECT_EQ(intersection.getIsHit(), true);
	EXPECT_EQ(intersection.getIntersectionPoint(), Tuple::Point(0, 0, -1));
	EXPECT_EQ(intersection.getDistance(), 4);
	EXPECT_EQ(intersection.getNormal(), Tuple::Vector(0, 0, -1));
}

TEST_F(SphereTests, The_ray_intersects_the_sphere_at_a_tangent) {
	Ray ray = Ray(Tuple::Point(0, 1, -5), Tuple::Vector(0, 0, 1));

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		this->transformation);

	IntersectionInfo intersection = sphere.Intersect(ray);

	EXPECT_EQ(intersection.getIsHit(), true);
	EXPECT_EQ(intersection.getIntersectionPoint(), Tuple::Point(0, 1, 0));
	EXPECT_EQ(intersection.getDistance(), 5);
	EXPECT_EQ(intersection.getNormal(), Tuple::Vector(0, 1, 0));
}

TEST_F(SphereTests, The_ray_misses_the_sphere) {
	Ray ray = Ray(Tuple::Point(0, 2, -5), Tuple::Vector(0, 0, 1));

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		this->transformation);

	IntersectionInfo intersection = sphere.Intersect(ray);

	EXPECT_EQ(intersection.getIsHit(), false);
}

TEST_F(SphereTests, The_ray_originates_inside_the_sphere) {
	//TODO: to look through and correct!
	Ray ray = Ray(Tuple::Point(0, 0, 0), Tuple::Vector(0, 0, 1));

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		this->transformation);

	IntersectionInfo intersection = sphere.Intersect(ray);

	EXPECT_EQ(intersection.getIsHit(), true);
	EXPECT_EQ(intersection.getIntersectionPoint(), Tuple::Point(0, 0, -1));
	EXPECT_EQ(intersection.getDistance(), -1);
	EXPECT_EQ(intersection.getNormal(), Tuple::Vector(0, 0, -1));
}

TEST_F(SphereTests, The_sphere_is_behind_the_ray) {
	Ray ray = Ray(Tuple::Point(0, 0, 5), Tuple::Vector(0, 0, 1));

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		this->transformation);

	IntersectionInfo intersection = sphere.Intersect(ray);

	EXPECT_EQ(intersection.getIsHit(), false);
}

TEST_F(SphereTests, The_sphere_transformation_is_modified) {
	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		Transformations::Translation(2, 3, 4));

	EXPECT_EQ(sphere.getInverseTransformation(), Transformations::Translation(2, 3, 4).Inverse());
}

TEST_F(SphereTests, The_ray_intersects_a_scaled_sphere) {

	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		Transformations::Scaling(2, 2, 2));

	IntersectionInfo intersection = sphere.Intersect(ray);

	EXPECT_EQ(intersection.getIsHit(), true);
	EXPECT_EQ(intersection.getIntersectionPoint(), Tuple::Point(0, 0, -2));
	EXPECT_EQ(intersection.getDistance(), 3);
	EXPECT_EQ(intersection.getNormal(), Tuple::Vector(0, 0, -1));
}

TEST_F(SphereTests, The_ray_intersects_a_translated_sphere) {
	// again on the tangent
	Ray ray = Ray(Tuple::Point(5, 1, -1), Tuple::Vector(0, 0, 1));

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		Transformations::Translation(4, 1, 0));

	IntersectionInfo intersection = sphere.Intersect(ray);

	EXPECT_EQ(intersection.getIsHit(), true);
	EXPECT_EQ(intersection.getIntersectionPoint(), Tuple::Point(5, 1, 0));
	EXPECT_EQ(intersection.getDistance(), 1);
	EXPECT_EQ(intersection.getNormal(), Tuple::Vector(1, 0, 0));
}

TEST_F(SphereTests, The_ray_intersects_a_translated_and_then_scaled_sphere) {
	Ray ray = Ray(Tuple::Point(4, 0, -1), Tuple::Vector(0, 0, 1));

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		Transformations::Translation(2, 0, 0) *
		Transformations::Scaling(2, 2, 2));

	IntersectionInfo intersection = sphere.Intersect(ray);

	EXPECT_EQ(intersection.getIsHit(), true);
	EXPECT_EQ(intersection.getIntersectionPoint(), Tuple::Point(4, 0, 0));
	EXPECT_EQ(intersection.getDistance(), 1);
	EXPECT_EQ(intersection.getNormal(), Tuple::Vector(1, 0, 0));
}

TEST_F(SphereTests, The_ray_intersects_a_scaled_and_then_translated_sphere) {
	Ray ray = Ray(Tuple::Point(6, 0, -1), Tuple::Vector(0, 0, 1));

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		Transformations::Scaling(2, 2, 2) *
		Transformations::Translation(2, 0, 0));

	IntersectionInfo intersection = sphere.Intersect(ray);

	EXPECT_EQ(intersection.getIsHit(), true);
	EXPECT_EQ(intersection.getIntersectionPoint(), Tuple::Point(6, 0, 0));
	EXPECT_EQ(intersection.getDistance(), 1);
	EXPECT_EQ(intersection.getNormal(), Tuple::Vector(1, 0, 0));
}

TEST_F(SphereTests, The_ray_misses_a_translated_sphere) {

	Ray ray = Ray(Tuple::Point(0, 0, -5), Tuple::Vector(0, 0, 1));

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		Transformations::Translation(5, 0, 0));

	IntersectionInfo intersection = sphere.Intersect(ray);

	EXPECT_EQ(intersection.getIsHit(), false);
}

TEST_F(SphereTests, The_normal_on_a_sphere_at_a_point_on_the_x_axis) {

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		this->transformation);

	EXPECT_EQ(sphere.getNormal(Tuple::Point(1, 0, 0)), Tuple::Point(1, 0, 0));
}

TEST_F(SphereTests, The_normal_on_a_sphere_at_a_point_on_the_y_axis) {

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		this->transformation);

	EXPECT_EQ(sphere.getNormal(Tuple::Point(0, 1, 0)), Tuple::Point(0, 1, 0));
}

TEST_F(SphereTests, The_normal_on_a_sphere_at_a_point_on_the_z_axis) {

	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		this->transformation);

	EXPECT_EQ(sphere.getNormal(Tuple::Point(0, 0, 1)), Tuple::Point(0, 0, 1));
}

TEST_F(SphereTests, The_normal_on_a_sphere_at_a_point_on_nonaxial_point) {

	float sqrt3_3 = sqrt(3) / 3;
	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		this->transformation);

	EXPECT_EQ(sphere.getNormal(Tuple::Point(sqrt3_3, sqrt3_3, sqrt3_3)),
		Tuple::Point(sqrt3_3, sqrt3_3, sqrt3_3));
}

TEST_F(SphereTests, The_normal_on_a_translated_sphere) {
	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		Transformations::Translation(0, 1, 0));

	EXPECT_EQ(sphere.getNormal(Tuple::Point(0, 1.70711, -0.70711)),
		Tuple::Vector(0, 0.70711, -0.70711));
}

TEST_F(SphereTests, The_normal_on_a_transformed_sphere) {
	Sphere sphere = Sphere(this->origin,
		this->radius,
		this->material,
		Transformations::Scaling(1, 0.5, 1) *
		Transformations::RotationZ(36));

	EXPECT_EQ(sphere.getNormal(Tuple::Point(0, sqrt(2) / 2, -sqrt(2) / 2)),
		Tuple::Vector(0, 0.97014, -0.24254));
}