#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)
#include "source\light.h"

TEST(LightTests, CameraBetweenLightAndSurface) {
	Light light = Light(Tuple::Point(0, 0, -10), Color(1, 1, 1), 1, 1, 1, 1);
	Material material = Material(Color(1, 1, 1), 0.1f, 0.9f, 0.9f, 200.0f);

	Color phongColor = light.getPhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1),
		Tuple::Vector(0, 0, -1), &material);
	EXPECT_EQ(phongColor, Color(1.9f, 1.9f, 1.9f));
}

TEST(LightTests, LightOppositeToSurfaceWithCameraOffset) {
	Light light = Light(Tuple::Point(0, 0, -10), Color(1, 1, 1), 1, 1, 1, 1);
	Material material = Material(Color(1, 1, 1), 0.1f, 0.9f, 0.9f, 200.0f);

	Color phongColor = light.getPhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).normalize(),
		Tuple::Vector(0, sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f).normalize(), &material);
	EXPECT_EQ(phongColor, Color(1.0f, 1.0f, 1.0f));
}

TEST(LightTests, CameraOppositeToSurfaceWithLightOffset) {
	Light light = Light(Tuple::Point(0, 10, -10), Color(1, 1, 1), 1, 1, 1, 1);
	Material material = Material(Color(1, 1, 1), 0.1f, 0.9f, 0.9f, 200.0f);

	Color phongColor = light.getPhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).normalize(),
		Tuple::Vector(0, 0, -1).normalize(),
		&material);
	EXPECT_EQ(phongColor, Color(0.7364f, 0.7364f, 0.7364f));
}

TEST(LightTests, CameraOnReflectionVector) {
	Light light = Light(Tuple::Point(0, 10, -10), Color(1, 1, 1), 1, 1, 1, 1);
	Material material = Material(Color(1, 1, 1), 0.1f, 0.9f, 0.9f, 200.0f);

	Color phongColor = light.getPhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).normalize(),
		Tuple::Vector(0, -sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f).normalize(),
		&material);
	EXPECT_EQ(phongColor, Color(1.6364f, 1.6364f, 1.6364f));
}

TEST(LightTests, LightBehindSurface) {
	Light light = Light(Tuple::Point(0, 0, 10), Color(1, 1, 1), 1, 1, 1, 1);
	Material material = Material(Color(1, 1, 1), 0.1f, 0.9f, 0.9f, 200.0f);

	Color phongColor = light.getPhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).normalize(),
		Tuple::Vector(0, 0, -1).normalize(),
		&material);
	EXPECT_EQ(phongColor, Color(0.1f, 0.1f, 0.1f));
}