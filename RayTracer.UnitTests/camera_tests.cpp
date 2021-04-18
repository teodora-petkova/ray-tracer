#include "gtest\gtest.h"
#include "glm\glm.hpp"
#include "Source\camera.h"

TEST(CameraTests, CorrectlySetOrigin) {
	Camera camera = Camera(Tuple::Vector(0, 0, 5), Tuple::Vector(0, 0, 0), Tuple::Vector(0, 1, 0), 60, 640, 480);
	EXPECT_EQ(camera.getOrigin(), Tuple::Point(0, 0, 5));
}

TEST(CameraTests, DirectionalRayFor00) {
	Camera camera = Camera(Tuple::Vector(0, 0, 1), Tuple::Vector(0, 0, 0), Tuple::Vector(0, 1, 1), 60, 1, 1);
	EXPECT_EQ(camera.getDirectionRayForPixel(0, 0), Tuple::Vector(0, 0, -1));
}