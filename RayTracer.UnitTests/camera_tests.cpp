#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#include <math.h>

#include "source\sphere.h"
#include "source\camera.h"

float round2(float value)
{
	return std::roundf(value * 100.0f) / 100.0f;
}

TEST(CameraTests, When_the_camera_is_correctly_initialised)
{
	Camera camera = Camera(Tuple::Vector(0, 0, -5), Tuple::Vector(0, 0, 0), Tuple::Vector(0, 1, 0), 90, 160, 120);
	EXPECT_EQ(camera.getWidth(), 160);
	EXPECT_EQ(camera.getHeight(), 120);
	EXPECT_FLOAT_EQ(round2(camera.getFOV()), 1.57f);
}

TEST(CameraTests, When_the_camera_origin_is_set_correctly) {
	Camera camera = Camera(Tuple::Vector(0, 0, -5), Tuple::Vector(0, 0, 0), Tuple::Vector(0, 1, 0), 60, 640, 480);
	EXPECT_EQ(camera.getOrigin(), Tuple::Point(0, 0, -5));
}

TEST(CameraTests, Correct_pixel_size_for_the_horizontal_canvas) {
	Tuple from = Tuple::Point(0, 0, 0);
	Tuple up = Tuple::Point(0, 1, 0);
	Tuple to = Tuple::Point(0, 0, -1);

	Camera c = Camera(from, to, up, 90, 200, 125);

	std::pair<float, float> pixelSize = c.getPixelSize();

	EXPECT_FLOAT_EQ(round2(pixelSize.first), 0.01f);
	EXPECT_FLOAT_EQ(round2(pixelSize.second), 0.01f);
}

TEST(CameraTests, Correct_pixel_size_for_the_vertical_canvas) {
	Tuple from = Tuple::Point(0, 0, 0);
	Tuple up = Tuple::Point(0, 1, 0);
	Tuple to = Tuple::Point(0, 0, -1);

	Camera c = Camera(from, to, up, 90, 125, 200);

	std::pair<float, float> pixelSize = c.getPixelSize();

	EXPECT_FLOAT_EQ(round2(pixelSize.first), 0.01f);
	EXPECT_FLOAT_EQ(round2(pixelSize.second), 0.01f);
}

TEST(CameraTests, Construct_a_ray_through_the_center_of_the_canvas) {
	Tuple from = Tuple::Point(0, 0, 0);
	Tuple up = Tuple::Point(0, 1, 0);
	Tuple to = Tuple::Point(0, 0, -1);

	Camera c = Camera(from, to, up, 90, 21, 11);
	Ray r = c.CalculateRayForPixel(10, 5);

	EXPECT_EQ(r.getOrigin(), Tuple::Point(0, 0, 0));
	EXPECT_EQ(r.getDirection(), Tuple::Vector(0, 0, -1));
}

TEST(CameraTests, Construct_a_ray_through_the_corner_of_the_canvas) {
	Tuple from = Tuple::Point(0, 0, 0);
	Tuple up = Tuple::Point(0, 1, 0);
	Tuple to = Tuple::Point(0, 0, -1);

	Camera c = Camera(from, to, up, 90, 201, 101);
	Ray r = c.CalculateRayForPixel(0, 0);

	EXPECT_EQ(r.getOrigin(), Tuple::Point(0, 0, 0));
	EXPECT_EQ(r.getDirection(), Tuple::Vector(0.66519f, 0.33259f, -0.66851f));
}

TEST(CameraTests, Construct_a_ray_when_the_camera_is_transformed) {
	Tuple from = Tuple::Point(0, 0, 0);
	Tuple up = Tuple::Point(0, 1, 0);
	Tuple to = Tuple::Point(0, 0, -1);

	Camera c = Camera(from, to, up, 90, 201, 101);
	Matrix<4, 4> transform = Transformations::RotationY(45) *
		Transformations::Translation(0, -2, 5);
	c.setTransform(transform);
	Ray r = c.CalculateRayForPixel(100, 50);

	EXPECT_EQ(r.getOrigin(), Tuple::Point(0, 2, -5));
	EXPECT_EQ(r.getDirection(), Tuple::Vector(sqrt(2) / 2.0f, 0, -sqrt(2) / 2.0f));
}