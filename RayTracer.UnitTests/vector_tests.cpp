#include "gtest\gtest.h"
#include "glm\glm.hpp"
#include "Source\tuple.h"

TEST(VectorTests, VectorIsEqualToItself) {
	Tuple v = Tuple::Vector(1, 2, 3);
	EXPECT_EQ(v, v);
}

TEST(VectorTests, DotProductOfTwoZeroVectorsIsZeros) {
	Tuple v1 = Tuple::Vector(0, 0, 0);
	Tuple v2 = Tuple::Vector(0, 0, 0);

	EXPECT_EQ(Tuple::dot(v1, v2), 0);
}

TEST(VectorTests, CrossProductOfTwoZeroVectorsIsZeroVector) {
	Tuple v1 = Tuple::Vector(0, 0, 0);
	Tuple v2 = Tuple::Vector(0, 0, 0);

	EXPECT_EQ(Tuple::cross(v1, v2), Tuple::Vector(0, 0, 0));
}

TEST(VectorTests, DotProductOfTwoNonZeroVectorsIsCorrect) {
	Tuple v1 = Tuple::Vector(1, 2, 3);
	Tuple v2 = Tuple::Vector(4, 5, 6);

	EXPECT_EQ(Tuple::dot(v1, v2), 4 + 10 + 18);
}

TEST(VectorTests, CrossProductOfTwoNonZeroVectorsIsCorrect) {
	Tuple v1 = Tuple::Vector(1, 2, 3);
	Tuple v2 = Tuple::Vector(4, 5, 6);

	EXPECT_EQ(Tuple::cross(v1, v2), Tuple::Vector(-3, 6, -3));
}