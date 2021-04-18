#include "gtest\gtest.h"
#include "glm\glm.hpp"
#include "Source\light.h"

TEST(LightTests, CameraBetweenLightAndSurface) {
	Light light = Light(Tuple::Point(0, 0, -10), Color(1, 1, 1), 1, 1, 1, 1);
	Material material = Material(Color(1, 1, 1), 0.1, 0.9, 0.9, 200.0);

	Color phongColor = light.getPhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1),
		Tuple::Vector(0, 0, -1), &material);
	EXPECT_EQ(phongColor, Color(1.9, 1.9, 1.9));
}

TEST(LightTests, LightOppositeToSurfaceWithCameraOffset) {
	Light light = Light(Tuple::Point(0, 0, -10), Color(1, 1, 1), 1, 1, 1, 1);
	Material material = Material(Color(1, 1, 1), 0.1, 0.9, 0.9, 200.0);

	Color phongColor = light.getPhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).normalize(),
		Tuple::Vector(0, sqrt(2) / 2, -sqrt(2) / 2).normalize(), &material);
	EXPECT_EQ(phongColor, Color(1, 1, 1));
}

TEST(LightTests, CameraOppositeToSurfaceWithLightOffset) {
	Light light = Light(Tuple::Point(0, 10, -10), Color(1, 1, 1), 1, 1, 1, 1);
	Material material = Material(Color(1, 1, 1), 0.1, 0.9, 0.9, 200.0);

	Color phongColor = light.getPhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).normalize(),
		Tuple::Vector(0, 0, -1).normalize(),
		&material);
	EXPECT_EQ(phongColor, Color(0.7364, 0.7364, 0.7364));
}

TEST(LightTests, CameraOnReflectionVector) {
	Light light = Light(Tuple::Point(0, 10, -10), Color(1, 1, 1), 1, 1, 1, 1);
	Material material = Material(Color(1, 1, 1), 0.1, 0.9, 0.9, 200.0);

	Color phongColor = light.getPhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).normalize(),
		Tuple::Vector(0, -sqrt(2) / 2.0f, -sqrt(2) / 2.0f).normalize(),
		&material);
	EXPECT_EQ(phongColor, Color(1.6364, 1.6364, 1.6364));
}

TEST(LightTests, LightBehindSurface) {
	Light light = Light(Tuple::Point(0, 0, 10), Color(1, 1, 1), 1, 1, 1, 1);
	Material material = Material(Color(1, 1, 1), 0.1, 0.9, 0.9, 200.0);

	Color phongColor = light.getPhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).normalize(),
		Tuple::Vector(0, 0, -1).normalize(),
		&material);
	EXPECT_EQ(phongColor, Color(0.1, 0.1, 0.1));
}
