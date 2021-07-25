#pragma warning(push, 0)
#include "gtest\gtest.h"
#pragma warning(pop)
#include "source\matrix.h"

TEST(MatrixTests, CorrectInitialization4by4) {
	Matrix<4, 4> m({
		{    1,    2,    3,    4 },
		{  5.5,  6.5,  7.5,  8.5 },
		{    9,   10,   11,   12 },
		{ 13.5, 14.5, 15.5, 16.5 } });

	EXPECT_EQ(m.getHeight(), 4);
	EXPECT_EQ(m.getWidth(), 4);

	EXPECT_EQ(m.At(0, 0), 1);
	EXPECT_EQ(m.At(0, 1), 2);
	EXPECT_EQ(m.At(0, 2), 3);
	EXPECT_EQ(m.At(0, 3), 4);

	EXPECT_EQ(m.At(1, 0), 5.5);
	EXPECT_EQ(m.At(1, 1), 6.5);
	EXPECT_EQ(m.At(1, 2), 7.5);
	EXPECT_EQ(m.At(1, 3), 8.5);

	EXPECT_EQ(m.At(2, 0), 9);
	EXPECT_EQ(m.At(2, 1), 10);
	EXPECT_EQ(m.At(2, 2), 11);
	EXPECT_EQ(m.At(2, 3), 12);

	EXPECT_EQ(m.At(3, 0), 13.5);
	EXPECT_EQ(m.At(3, 1), 14.5);
	EXPECT_EQ(m.At(3, 2), 15.5);
	EXPECT_EQ(m.At(3, 3), 16.5);
}

TEST(MatrixTests, CorrectInitialization3by3) {
	Matrix<3, 3> m({
		{-3,  5,  0},
		{ 1, -2, -7},
		{ 7,  9, -6} });

	EXPECT_EQ(m.getHeight(), 3);
	EXPECT_EQ(m.getWidth(), 3);

	EXPECT_EQ(m.At(0, 0), -3);
	EXPECT_EQ(m.At(0, 1), 5);
	EXPECT_EQ(m.At(0, 2), 0);

	EXPECT_EQ(m.At(1, 0), 1);
	EXPECT_EQ(m.At(1, 1), -2);
	EXPECT_EQ(m.At(1, 2), -7);

	EXPECT_EQ(m.At(2, 0), 7);
	EXPECT_EQ(m.At(2, 1), 9);
	EXPECT_EQ(m.At(2, 2), -6);
}

TEST(MatrixTests, CorrectInitialization2by2) {
	Matrix<2, 2> m({
		{ -3,  5},
		{  1, -2} });

	EXPECT_EQ(m.getHeight(), 2);
	EXPECT_EQ(m.getWidth(), 2);

	EXPECT_EQ(m.At(0, 0), -3);
	EXPECT_EQ(m.At(0, 1), 5);
	EXPECT_EQ(m.At(1, 0), 1);
	EXPECT_EQ(m.At(1, 1), -2);
}

TEST(MatrixTests, EqualMatrices)
{
	Matrix<4, 4> m1({
		{  1,  2,  3,  4 },
		{  5,  6,  7,  8 },
		{  9, 10, 11, 12 },
		{ 13, 14, 15, 16 } });

	Matrix<4, 4> m2({
		{  1,  2,  3,  4 },
		{  5,  6,  7,  8 },
		{  9, 10, 11, 12 },
		{ 13, 14, 15, 16 } });

	EXPECT_EQ(m1, m2);
}

TEST(MatrixTests, DifferentMatrices)
{
	Matrix<4, 4> m1({
		{  1,  2,  3,  4 },
		{  5,  6,  7,  8 },
		{  9, 10, 11, 12 },
		{ 13, 14, 15, 16 } });

	Matrix<4, 4> m2({
		{  1,  2,  3,  4 },
		{  5,  6,  7,  8 },
		{  9, 10, 11, 12 },
		{  1,  2,  3,  4 } });

	EXPECT_NE(m1, m2);
}

TEST(MatrixTests, MultipliedMatrices4x4by4x4)
{
	Matrix<4, 4> m1({
		{  1,  2,  3,  4 },
		{  5,  6,  7,  8 },
		{  9,  8,  7,  6 },
		{  5,  4,  3,  2 } });

	Matrix<4, 4> m2({
		{ -2,  1,  2,  3 },
		{  3,  2,  1, -1 },
		{  4,  3,  6,  5 },
		{  1,  2,  7,  8 } });

	Matrix<4, 4> m3({
		{  20, 22,  50,  48 },
		{  44, 54, 114, 108 },
		{  40, 58, 110, 102 },
		{  16, 26,  46,  42 } });

	Matrix<4, 4> m = m1 * m2;

	EXPECT_EQ(m, m3);
}

TEST(MatrixTests, MultipliedMatrices2x3by3x2)
{
	Matrix<2, 3> m1({
		{  1,  2,  3 },
		{  4,  5,  6 } });

	Matrix<3, 2> m2({
		{  7,  8 },
		{  9, 10 },
		{ 11, 12 } });

	Matrix<2, 2> m3({
		{  58,  64 },
		{ 139, 154 } });

	Matrix<2, 2> m = m1 * m2;

	EXPECT_EQ(m, m3);
}

TEST(MatrixTests, MultipliedMatriceByTuple)
{
	Matrix<4, 4> m({
		{  1,  2,  3,  4 },
		{  2,  4,  4,  2 },
		{  8,  6,  4,  1 },
		{  0,  0,  0,  1 } });

	Tuple t = Tuple(1, 2, 3, 1);

	Tuple expectedResult = Tuple(18, 24, 33, 1);
	Tuple result = m * t;
	EXPECT_EQ(result, expectedResult);
}

TEST(MatrixTests, MatrixMultipliedByIdentityMatrix)
{
	Matrix<4, 4> m({
		{  0,  1,  2,  4 },
		{  1,  2,  4,  8 },
		{  2,  4,  8, 16 },
		{  4,  8, 16, 32 } });

	Matrix<4, 4> result = m * Matrix<4, 4>::IdentityMatrix();
	EXPECT_EQ(result, m);
}

TEST(MatrixTests, IdentityMatrixMultipliedByTuple)
{
	Tuple t = Tuple(1, 2, 3, 4);

	Tuple result = Matrix<4, 4>::IdentityMatrix() * t;
	EXPECT_EQ(result, t);
}

TEST(MatrixTests, TransposeMatrix2by3to3by2)
{
	Matrix<2, 3> m({
			{  6,  4,  24 },
			{  1, -9,   8 } });

	Matrix<3, 2> expectedResult({
			{  6,  1 },
			{  4, -9 },
			{ 24,  8 } });

	Matrix<3, 2> result = m.Transpose();

	EXPECT_EQ(result, expectedResult);
}

TEST(MatrixTests, TransposeIndetityMatrix)
{
	Matrix<3, 3> id = Matrix<3, 3>::IdentityMatrix();
	Matrix<3, 3> result = id.Transpose();

	EXPECT_EQ(result, id);
}

TEST(MatrixTests, DeterminantOfMatrix2By2)
{
	Matrix<2, 2> m({ { 1, 5},
					 {-3, 2 } });

	EXPECT_EQ(m.Determinant(), 17);
}

TEST(MatrixTests, Submatrix2by2OfMatrix3by3)
{
	Matrix<3, 3> m3by3({
		{  1,  5,  0 },
		{ -3,  2,  7 },
		{  0,  6, -3 } });

	Matrix<2, 2> m2by2({
		{  -3,  2 },
		{   0,  6 } });

	EXPECT_EQ(m3by3.SubMatrix(0, 2), m2by2);
}

TEST(MatrixTests, Submatrix3by3OfMatrix4by4)
{
	Matrix<4, 4> m4by4({
		{ -6,  1,  1,  6 },
		{ -8,  5,  8,  6 },
		{ -1,  0,  8,  2 },
		{ -7,  1, -1,  1 } });

	Matrix<3, 3> m3by3({
		{  -6,  1,  6 },
		{  -8,  8,  6 },
		{  -7, -1,  1 } });

	EXPECT_EQ(m4by4.SubMatrix(2, 1), m3by3);
}

TEST(MatrixTests, MinorOfMatrix3by3)
{
	Matrix<3, 3> m3by3({
		{  3,  5,  0 },
		{  2, -1, -7 },
		{  6, -1,  5 } });

	Matrix<2, 2> m2by2 = m3by3.SubMatrix(1, 0);

	EXPECT_EQ(m2by2.Determinant(), 25);
	EXPECT_EQ(m3by3.Minor(1, 0), 25);
}

TEST(MatrixTests, CofactorOfMatrix3by3)
{
	Matrix<3, 3> m3by3({
		{  3,  5,  0 },
		{  2, -1, -7 },
		{  6, -1,  5 } });

	Matrix<2, 2> m2by2 = m3by3.SubMatrix(1, 0);

	EXPECT_EQ(m3by3.Minor(0, 0), -12);
	EXPECT_EQ(m3by3.Cofactor(0, 0), -12);

	EXPECT_EQ(m3by3.Minor(1, 0), 25);
	EXPECT_EQ(m3by3.Cofactor(1, 0), -25);
}

TEST(MatrixTests, DeterminantOfMatrix3by3)
{
	Matrix<3, 3> m3by3({
		{  1,  2,  6 },
		{ -5,  8, -4 },
		{  2,  6,  4 } });

	EXPECT_EQ(m3by3.Cofactor(0, 0), 56);
	EXPECT_EQ(m3by3.Cofactor(0, 1), 12);
	EXPECT_EQ(m3by3.Cofactor(0, 2), -46);
	EXPECT_EQ(m3by3.Determinant(), -196);
}

TEST(MatrixTests, DeterminantOfMatrix4by4)
{
	Matrix<4, 4> m4by4({
		{ -2, -8,  3,  5 },
		{ -3,  1,  7,  3 },
		{  1,  2, -9,  6 },
		{ -6,  7,  7, -9 } });

	EXPECT_EQ(m4by4.Cofactor(0, 0), 690);
	EXPECT_EQ(m4by4.Cofactor(0, 1), 447);
	EXPECT_EQ(m4by4.Cofactor(0, 2), 210);
	EXPECT_EQ(m4by4.Cofactor(0, 3), 51);
	EXPECT_EQ(m4by4.Determinant(), -4071);
}

TEST(MatrixTests, InvertibleMatrix)
{
	Matrix<4, 4> m4by4({
		{  6,  4,  4,  4 },
		{  5,  5,  7,  6 },
		{  4, -9,  3, -7 },
		{  9,  1,  7, -6 } });

	EXPECT_EQ(m4by4.Determinant(), -2120);
	EXPECT_EQ(m4by4.IsInvertible(), true);
}

TEST(MatrixTests, NoninvertibleMatrix)
{
	Matrix<4, 4> m4by4({
		{ -4,  2, -2, -3 },
		{  9,  6,  2,  6 },
		{  0, -5,  1, -5 },
		{  0,  0,  0,  0 } });

	EXPECT_EQ(m4by4.Determinant(), 0);
	EXPECT_EQ(m4by4.IsInvertible(), false);
}

TEST(MatrixTests, CalculateInverseOfMatrix1)
{
	Matrix<4, 4> m4by4({
		{ -5,  2,  6, -8 },
		{  1, -5,  1,  8 },
		{  7,  7, -6, -7 },
		{  1, -3,  7,  4 } });
	Matrix<4, 4> inverseMatrix = m4by4.Inverse();

	EXPECT_EQ(m4by4.Determinant(), 532);

	EXPECT_EQ(m4by4.Cofactor(2, 3), -160);
	EXPECT_EQ(inverseMatrix.At(3, 2), float(-160) / 532);


	EXPECT_EQ(m4by4.Cofactor(3, 2), 105);
	EXPECT_EQ(inverseMatrix.At(2, 3), float(105) / 532);

	Matrix<4, 4> expectedResult({
		{  0.21805f,  0.45113f,  0.24060f, -0.04511f },
		{ -0.80827f, -1.45677f, -0.44361f,  0.52068f },
		{ -0.07895f, -0.22368f, -0.05263f,  0.19737f },
		{ -0.52256f, -0.81391f, -0.30075f,  0.30639f } });
	EXPECT_EQ(inverseMatrix, expectedResult);
}

TEST(MatrixTests, CalculateInverseOfMatrix2)
{
	Matrix<4, 4> m4by4({
		{  8, -5,  9,  2 },
		{  7,  5,  6,  1 },
		{ -6,  0,  9,  6 },
		{ -3,  0, -9, -4 } });

	Matrix<4, 4> inverseMatrix = m4by4.Inverse();

	Matrix<4, 4> expectedResult({
		{ -0.15385f, -0.15385f, -0.28205f, -0.53846f },
		{ -0.07692f,  0.12308f,  0.02564f,  0.03077f },
		{  0.35897f,  0.35897f,  0.43590f,  0.92308f },
		{ -0.69231f, -0.69231f, -0.76923f, -1.92308f } });

	EXPECT_EQ(inverseMatrix, expectedResult);
}

TEST(MatrixTests, CalculateInverseOfMatrix3)
{
	Matrix<4, 4> m4by4({
		{  9,  3,  0,  9 },
		{ -5, -2, -6, -3 },
		{ -4,  9,  6,  4 },
		{ -7,  6,  6,  2 } });

	Matrix<4, 4> inverseMatrix = m4by4.Inverse();

	Matrix<4, 4> expectedResult({

		{ -0.04074f, -0.07778f,  0.14444f, -0.22222f },
		{ -0.07778f,  0.03333f,  0.36667f, -0.33333f },
		{ -0.02901f, -0.14630f, -0.10926f,  0.12963f },
		{  0.17778f,  0.06667f, -0.26667f,  0.33333f } });

	EXPECT_EQ(inverseMatrix, expectedResult);
}

TEST(MatrixTests, MultiplyAProductByItsInverse)
{
	Matrix<4, 4> A({
		{  3, -9,  7,  3 },
		{  3, -8,  2, -9 },
		{ -4,  4,  4,  1 },
		{ -6,  5, -1,  1 } });

	Matrix<4, 4> B({
		{ 8, 2, 2, 2 },
		{ 3, -1, 7, 0 },
		{ 7, 0, 5, 4 },
		{ 6, -2, 0, 5 } });

	Matrix<4, 4> C = A * B;
	Matrix<4, 4> inverseB = B.Inverse();
	Matrix<4, 4> id = Matrix<4, 4>::IdentityMatrix();

	EXPECT_EQ(C * inverseB, A);
	EXPECT_EQ(B * inverseB, id);
}

TEST(MatrixTests, MultiplyAMatrixByItsInverse)
{
	Matrix<4, 4> A({
		{  3, -9,  7,  3 },
		{  3, -8,  2, -9 },
		{ -4,  4,  4,  1 },
		{ -6,  5, -1,  1 } });

	Matrix<4, 4> id = Matrix<4, 4>::IdentityMatrix();

	EXPECT_EQ(A * A.Inverse(), id);
}

TEST(MatrixTests, InverseOfIdentity)
{
	Matrix<4, 4> id = Matrix<4, 4>::IdentityMatrix();

	EXPECT_EQ(id.Inverse(), id);
}

TEST(MatrixTests, InverseAndTranspose)
{
	Matrix<4, 4> A({
		{  3, -9,  7,  3 },
		{  3, -8,  2, -9 },
		{ -4,  4,  4,  1 },
		{ -6,  5, -1,  1 } });

	EXPECT_EQ(A.Inverse().Transpose(), A.Transpose().Inverse());
}
