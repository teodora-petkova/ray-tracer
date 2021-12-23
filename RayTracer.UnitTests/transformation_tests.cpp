#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)

#include "source\tuple.h"
#include "source\transformations.h"

TEST(TransformationsTests, TranslateAPoint) {
	Matrix<4, 4> translation = Transformations::Translation(5, -3, 2);
	Tuple point = Tuple::Point(-3, 4, 5);
	EXPECT_EQ(translation * point, Tuple::Point(2, 1, 7));
}

TEST(TransformationsTests, InverseTranslateAPoint) {
	Matrix<4, 4> inverseTranslation = Transformations::Translation(5, -3, 2).Inverse();
	Tuple point = Tuple::Point(-3, 4, 5);
	EXPECT_EQ(inverseTranslation * point, Tuple::Point(-8, 7, 3));
}

TEST(TransformationsTests, VectorCannotBeTranslated) {
	Matrix<4, 4> translation = Transformations::Translation(5, -3, 2);
	Tuple vector = Tuple::Vector(-3, 4, 5);
	EXPECT_EQ(translation * vector, vector);
}

TEST(TransformationsTests, ScaleAPoint) {
	Matrix<4, 4> scale = Transformations::Scaling(2, 3, 4);
	Tuple point = Tuple::Point(-4, 6, 8);
	EXPECT_EQ(scale * point, Tuple::Point(-8, 18, 32));
}

TEST(TransformationsTests, ScaleAVector) {
	Matrix<4, 4> scale = Transformations::Scaling(2, 3, 4);
	Tuple vector = Tuple::Vector(-4, 6, 8);
	EXPECT_EQ(scale * vector, Tuple::Vector(-8, 18, 32));
}

TEST(TransformationsTests, InverseScaleAVector) {
	Matrix<4, 4> inverseScale = Transformations::Scaling(2, 3, 4).Inverse();
	Tuple vector = Tuple::Vector(-4, 6, 8);
	EXPECT_EQ(inverseScale * vector, Tuple::Vector(-2, 2, 2));
}

TEST(TransformationsTests, RotateAPointByX) {
	Matrix<4, 4> rotateHalfQuarter = Transformations::RotationX(45);
	Matrix<4, 4> rotateFullQuarter = Transformations::RotationX(90);

	Tuple point = Tuple::Point(0, 1, 0);
	EXPECT_EQ(rotateHalfQuarter * point, Tuple::Point(0, sqrtf(2) / 2, sqrtf(2) / 2));
	EXPECT_EQ(rotateFullQuarter * point, Tuple::Point(0, 0, 1));
}

TEST(TransformationsTests, InverseRotateAPointByX) {
	Matrix<4, 4> inverseRotateHalfQuarter = Transformations::RotationX(45).Inverse();

	Tuple point = Tuple::Point(0, 1, 0);
	EXPECT_EQ(inverseRotateHalfQuarter * point, Tuple::Point(0, sqrtf(2) / 2, -sqrtf(2) / 2));
}

TEST(TransformationsTests, RotateAPointByY) {
	Matrix<4, 4> rotateHalfQuarter = Transformations::RotationY(45);
	Matrix<4, 4> rotateFullQuarter = Transformations::RotationY(90);

	Tuple point = Tuple::Point(0, 0, 1);
	EXPECT_EQ(rotateHalfQuarter * point, Tuple::Point(sqrtf(2) / 2, 0, sqrtf(2) / 2));
	EXPECT_EQ(rotateFullQuarter * point, Tuple::Point(1, 0, 0));
}

TEST(TransformationsTests, RotateAPointByZ) {
	Matrix<4, 4> rotateHalfQuarter = Transformations::RotationZ(45);
	Matrix<4, 4> rotateFullQuarter = Transformations::RotationZ(90);

	Tuple point = Tuple::Point(1, 0, 0);
	EXPECT_EQ(rotateHalfQuarter * point, Tuple::Point(sqrtf(2) / 2, sqrtf(2) / 2, 0));
	EXPECT_EQ(rotateFullQuarter * point, Tuple::Point(0, 1, 0));
}

TEST(TransformationsTests, ShearXtoY) {
	Matrix<4, 4> shear = Transformations::Shearing(1, 0, 0, 0, 0, 0);
	Tuple point = Tuple::Point(2, 3, 4);
	EXPECT_EQ(shear * point, Tuple::Point(5, 3, 4));
}

TEST(TransformationsTests, ShearXtoZ) {
	Matrix<4, 4> shear = Transformations::Shearing(0, 1, 0, 0, 0, 0);
	Tuple point = Tuple::Point(2, 3, 4);
	EXPECT_EQ(shear * point, Tuple::Point(6, 3, 4));
}

TEST(TransformationsTests, ShearYtoX) {
	Matrix<4, 4> shear = Transformations::Shearing(0, 0, 1, 0, 0, 0);
	Tuple point = Tuple::Point(2, 3, 4);
	EXPECT_EQ(shear * point, Tuple::Point(2, 5, 4));
}

TEST(TransformationsTests, ShearYtoZ) {
	Matrix<4, 4> shear = Transformations::Shearing(0, 0, 0, 1, 0, 0);
	Tuple point = Tuple::Point(2, 3, 4);
	EXPECT_EQ(shear * point, Tuple::Point(2, 7, 4));
}

TEST(TransformationsTests, ShearZtoX) {
	Matrix<4, 4> shear = Transformations::Shearing(0, 0, 0, 0, 1, 0);
	Tuple point = Tuple::Point(2, 3, 4);
	EXPECT_EQ(shear * point, Tuple::Point(2, 3, 6));
}

TEST(TransformationsTests, ShearZtoY) {
	Matrix<4, 4> shear = Transformations::Shearing(0, 0, 0, 0, 0, 1);
	Tuple point = Tuple::Point(2, 3, 4);
	EXPECT_EQ(shear * point, Tuple::Point(2, 3, 7));
}

TEST(TransformationsTests, RotateScaleAndTranslateAPoint) {
	Tuple p = Tuple::Point(1, 0, 1);

	Matrix<4, 4> A = Transformations::RotationX(90);
	Matrix<4, 4> B = Transformations::Scaling(5, 5, 5);
	Matrix<4, 4> C = Transformations::Translation(10, 5, 7);

	Tuple p2 = A * p;
	Tuple p3 = B * p2;
	Tuple p4 = C * p3;

	EXPECT_EQ(p2, Tuple::Point(1, -1, 0));
	EXPECT_EQ(p3, Tuple::Point(5, -5, 0));
	EXPECT_EQ(p4, Tuple::Point(15, 0, 7));
}

TEST(TransformationsTests, TranslateScaleAndRotateAPoint) {
	Tuple p = Tuple::Point(1, 0, 1);

	Matrix<4, 4> A = Transformations::RotationX(90);
	Matrix<4, 4> B = Transformations::Scaling(5, 5, 5);
	Matrix<4, 4> C = Transformations::Translation(10, 5, 7);

	Matrix<4, 4> T = C * B * A;

	EXPECT_EQ(T * p, Tuple::Point(15, 0, 7));
}