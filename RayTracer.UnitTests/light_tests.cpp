#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)
#include "source\light.h"

class LightTests : public ::testing::Test
{
protected:
	MaterialPtr material;

	LightTests() {
	}

	virtual ~LightTests() {
	}

protected:
	void SetUp() override {
		this->material = std::make_shared<Material>(Material(Color::White(), 0.1f, 0.9f, 0.9f, 200.0f));
	}
};

TEST_F(LightTests, CameraBetweenLightAndSurface) {
	Light light = Light(Tuple::Point(0, 0, -10), Color::White(), 1, 1, 1, 1);

	Color phongColor = light.CalculatePhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1),
		Tuple::Vector(0, 0, -1),
		this->material);
	EXPECT_EQ(phongColor, Color(1.9f, 1.9f, 1.9f));
}

TEST_F(LightTests, LightOppositeToSurfaceWithCameraOffset) {
	Light light = Light(Tuple::Point(0, 0, -10));

	Color phongColor = light.CalculatePhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).Normalize(),
		Tuple::Vector(0, sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f).Normalize(),
		this->material);
	EXPECT_EQ(phongColor, Color::White());
}

TEST_F(LightTests, CameraOppositeToSurfaceWithLightOffset) {
	Light light = Light(Tuple::Point(0, 10, -10));

	Color phongColor = light.CalculatePhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).Normalize(),
		Tuple::Vector(0, 0, -1).Normalize(),
		this->material);
	EXPECT_EQ(phongColor, Color(0.7364f, 0.7364f, 0.7364f));
}

TEST_F(LightTests, CameraOnReflectionVector) {
	Light light = Light(Tuple::Point(0, 10, -10));

	Color phongColor = light.CalculatePhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).Normalize(),
		Tuple::Vector(0, -sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f).Normalize(),
		this->material);
	EXPECT_EQ(phongColor, Color(1.6364f, 1.6364f, 1.6364f));
}

TEST_F(LightTests, LightBehindSurface) {
	Light light = Light(Tuple::Point(0, 0, 10));

	Color phongColor = light.CalculatePhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).Normalize(),
		Tuple::Vector(0, 0, -1).Normalize(),
		this->material);
	EXPECT_EQ(phongColor, Color(0.1f, 0.1f, 0.1f));
}