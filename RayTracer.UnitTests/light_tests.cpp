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

/*
* Both camera and light are on the normal of the surface,
* so all the components are with full coefficients
* 0.1(ambient) + 0.9(diffuse) + 0.9(specular) = 1.9
*/
TEST_F(LightTests, Camera_and_eye_on_the_normal_of_the_surface) {
	Light light = Light(Tuple::Point(0, 0, -10), Color::White(), 1, 1, 1, 1);

	Color phongColor = light.CalculatePhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1),
		Tuple::Vector(0, 0, -1),
		this->material, false);
	EXPECT_EQ(phongColor, Color(1.9f, 1.9f, 1.9f));
}

/*
* Light is on the normal of the surface, so ambient and diffuse stay the same
* but as the camera is at 45 degrees angle, the specular became near 0 (0.707 ^ 200 = 7.889e-13)
* 0.1(ambient) + 0.9(diffuse) + 0(specular) = 1
* // sqrt(2)/2 = 0.707
*/
TEST_F(LightTests, Light_is_on_the_normal_of_the_surface_but_camera_is_at_an_angle) {
	Light light = Light(Tuple::Point(0, 0, -10));

	Color phongColor = light.CalculatePhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).Normalize(),
		Tuple::Vector(0, sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f).Normalize(),
		this->material, false);
	EXPECT_EQ(phongColor, Color::White());
}

/*
* Camera is on the normal of the surface and the light is at 45 degrees angle,
* the specular again became near 0 (0.707 ^ 200 = 7.889e-13)
* and the diffuse is multiplied by dot(L, N) = sqrt(2)/2
* 0.1(ambient) + 0.9*(sqrt(2)/2)(diffuse) + 0(specular) = 0.7364
*/
TEST_F(LightTests, Camera_is_on_the_normal_of_the_surface_but_light_is_at_an_angle) {
	Light light = Light(Tuple::Point(0, 10, -10));

	Color phongColor = light.CalculatePhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).Normalize(),
		Tuple::Vector(0, 0, -1).Normalize(),
		this->material, false);
	EXPECT_EQ(phongColor, Color(0.7364f, 0.7364f, 0.7364f));
}

/*
* Camera is on the reflection vector and the light is at 45 degrees angle,
* the specular is 0.9
* and the diffuse is multiplied by dot(L, N) = sqrt(2)/2
* 0.1(ambient) + 0.9*(sqrt(2)/2)(diffuse) + 0.9(specular) = 1.6364
*/
TEST_F(LightTests, Camera_is_on_the_reflection_vector_and_light_is_at_angle) {
	Light light = Light(Tuple::Point(0, 10, -10));

	Color phongColor = light.CalculatePhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).Normalize(),
		Tuple::Vector(0, -sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f).Normalize(),
		this->material, false);
	EXPECT_EQ(phongColor, Color(1.6364f, 1.6364f, 1.6364f));
}

/*
* The light no longer illuminates the surface,
* the diffuse and specular components go to 0.
* 0.1(ambient) + 0(diffuse) + 0(specular) = 0.1
*/
TEST_F(LightTests, Light_is_behind_the_surface) {
	Light light = Light(Tuple::Point(0, 0, 10));

	Color phongColor = light.CalculatePhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1).Normalize(),
		Tuple::Vector(0, 0, -1).Normalize(),
		this->material, false);
	EXPECT_EQ(phongColor, Color(0.1f, 0.1f, 0.1f));
}

TEST_F(LightTests, Camera_and_eye_on_the_normal_of_the_surface_but_the_surface_is_in_shadow) {
	Light light = Light(Tuple::Point(0, 0, -10),
		Color::White(), 1, 1, 1, 1);

	Color phongColor = light.CalculatePhongColor(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1),
		Tuple::Vector(0, 0, -1),
		this->material, true);
	EXPECT_EQ(phongColor, Color(0.1f, 0.1f, 0.1f));
}