#include "gtest\gtest.h"
#include "glm\glm.hpp"
#include "Source\camera.h"

TEST(TestCamera, CorrectlySetOrigin) {
	Camera camera = Camera(Vector3(0, 0, 5), Vector3(0, 0, 0), Vector3(0, 1, 0), 60, 640, 480);
	EXPECT_EQ(camera.getOrigin(), Vector3(0, 0, 5));
}

TEST(TestCamera, DirectionalRayFor00) {
	Camera camera = Camera(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 1), 60, 1, 1);
	EXPECT_EQ(camera.getDirectionRayForPixel(0, 0), Vector3(0, 0, -1));
}