#include "gtest\gtest.h"
#include "glm\glm.hpp"
#include "Source\vector3.h"

TEST(VectorTests, VectorIsEqualToItself) {
	Vector3 v = Vector3(1, 2, 3);
	EXPECT_EQ(v, v);
}

TEST(VectorTests, DotProductOfTwoZeroVectorsIsZeros) {
	Vector3 v1 = Vector3(0, 0, 0);
	Vector3 v2 = Vector3(0, 0, 0);

	EXPECT_EQ(Vector3::dot(v1, v2), 0);
}

TEST(VectorTests, CrossProductOfTwoZeroVectorsIsZeroVector) {
	Vector3 v1 = Vector3(0, 0, 0);
	Vector3 v2 = Vector3(0, 0, 0);

	EXPECT_EQ(Vector3::cross(v1, v2), Vector3(0, 0, 0));
}

TEST(VectorTests, DotProductOfTwoNonZeroVectorsIsCorrect) {
	Vector3 v1 = Vector3(1, 2, 3);
	Vector3 v2 = Vector3(4, 5, 6);

	EXPECT_EQ(Vector3::dot(v1, v2), 4 + 10 + 18);
}

TEST(VectorTests, CrossProductOfTwoNonZeroVectorsIsCorrect) {
	Vector3 v1 = Vector3(1, 2, 3);
	Vector3 v2 = Vector3(4, 5, 6);

	EXPECT_EQ(Vector3::cross(v1, v2), Vector3(-3, 6, -3));
}