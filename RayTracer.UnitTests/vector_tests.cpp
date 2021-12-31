#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)
#include "source\tuple.h"

TEST(VectorTests, VectorIsEqualToItself) {
	Tuple v = Tuple::Vector(1, 2, 3);
	EXPECT_EQ(v, v);
}

TEST(VectorTests, DotProductOfTwoZeroVectorsIsZeros) {
	Tuple v1 = Tuple::Vector(0, 0, 0);
	Tuple v2 = Tuple::Vector(0, 0, 0);

	EXPECT_EQ(v1.Dot(v2), 0);
}

TEST(VectorTests, CrossProductOfTwoZeroVectorsIsZeroVector) {
	Tuple v1 = Tuple::Vector(0, 0, 0);
	Tuple v2 = Tuple::Vector(0, 0, 0);

	EXPECT_EQ(v1.Cross(v2), Tuple::Vector(0, 0, 0));
}

TEST(VectorTests, DotProductOfTwoNonZeroVectorsIsCorrect) {
	Tuple v1 = Tuple::Vector(1, 2, 3);
	Tuple v2 = Tuple::Vector(4, 5, 6);

	EXPECT_EQ(v1.Dot(v2), 4 + 10 + 18);
}

TEST(VectorTests, CrossProductOfTwoNonZeroVectorsIsCorrect) {
	Tuple v1 = Tuple::Vector(1, 2, 3);
	Tuple v2 = Tuple::Vector(4, 5, 6);

	EXPECT_EQ(v1.Cross(v2), Tuple::Vector(-3, 6, -3));
}

TEST(VectorTests, CrossProductWithNegativeCoordinatesIsCorrect) {
	Tuple v1 = Tuple::Vector(-2, 0, 0);
	Tuple v2 = Tuple::Vector(-1, 0.5, 0);

	EXPECT_EQ(v1.Cross(v2), Tuple::Vector(0, 0, -1));
}

TEST(VectorTests, Reflect_a_vector) {
	float sqrt2over2 = sqrt(2) / 2;
	Tuple v1 = Tuple::Vector(0, -sqrt2over2, sqrt2over2);
	Tuple normal = Tuple::Vector(0, 1, 0);

	EXPECT_EQ(v1.Reflect(normal), Tuple::Vector(0, sqrt2over2, sqrt2over2));
}