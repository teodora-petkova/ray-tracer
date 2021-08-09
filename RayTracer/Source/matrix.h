#ifndef MATRIX_H
#define MATRIX_H

#pragma once

#include "raytracer_exports.h"
#include "color.h"

template<uint8_t ROWS, uint8_t COLUMNS>
class RAYTRACER_EXPORT Matrix
{
public:
	Matrix()
	{
	}

	Matrix(std::initializer_list<std::initializer_list<float>> list)
	{
		uint8_t rows = 0;
		uint8_t cols = 0;

		for (auto row : list)
		{
			if (rows >= ROWS) break;
			for (auto element : row)
			{
				if (cols >= COLUMNS) break;
				this->data[rows][cols++] = element;
			}
			cols = 0;
			rows++;
		}
	}

	int getWidth() const { return ROWS; }
	int getHeight() const { return COLUMNS; }

	const float At(uint8_t row, uint8_t col) const { return this->data[row][col]; }
	float& At(uint8_t row, uint8_t col) { return this->data[row][col]; }

	bool operator==(const Matrix<ROWS, COLUMNS>& m2) const
	{
		for (uint8_t i = 0; i < this->getWidth(); i++)
		{
			for (uint8_t j = 0; j < this->getHeight(); j++)
			{
				if (!equals(this->At(i, j), m2.At(i, j)))
					return false;
			}
		}
		return true;
	}

	bool operator!=(const Matrix<ROWS, COLUMNS>& m2) const
	{
		return !this->operator==(m2);
	}

	template<uint8_t ROWS2, uint8_t COLUMNS2>
	Matrix<ROWS, COLUMNS2> operator*(const Matrix<ROWS2, COLUMNS2>& m2) const
	{
		Matrix<ROWS, COLUMNS2> m;

		static_assert((COLUMNS == ROWS2), "Matrix multiplication requires for the number of columns in the first matrix to be equal to the number of rows in the second matrix.");

		for (uint8_t i = 0; i < ROWS; i++)
		{
			for (uint8_t j = 0; j < COLUMNS2; j++)
			{
				float sum = 0;
				for (uint8_t k = 0; k < COLUMNS; k++)
				{
					sum += this->At(i, k) * m2.At(k, j);
				}

				m.At(i, j) = sum;
			}
		}

		return m;
	}

	Tuple operator*(const Tuple& t) const
	{
		static_assert((ROWS == 4 && COLUMNS == 4), "Matrix-Tuple multiplication requires for a 4 by 4 Matrix.");

		Matrix<4, 1> tupleMatrix({ {t.X()}, {t.Y()}, {t.Z()}, {t.W()} });

		Matrix<4, 1> m = (*this) * tupleMatrix;

		return Tuple(m.At(0, 0), m.At(1, 0), m.At(2, 0), m.At(3, 0));
	}

	static const Matrix<ROWS, COLUMNS> IdentityMatrix()
	{
		Matrix<ROWS, COLUMNS> m;

		for (uint8_t i = 0; i < ROWS; i++)
		{
			for (uint8_t j = 0; j < COLUMNS; j++)
			{
				m.At(i, j) = 0;
				if (i == j)
				{
					m.At(i, j) = 1;
				}
			}
		}
		return m;
	}

	Matrix<COLUMNS, ROWS> Transpose() const
	{
		Matrix<COLUMNS, ROWS> m;

		for (uint8_t i = 0; i < ROWS; i++)
		{
			for (uint8_t j = 0; j < COLUMNS; j++)
			{
				m.At(j, i) = this->At(i, j);
			}
		}

		return m;
	}

	Matrix<ROWS - 1, COLUMNS - 1> SubMatrix(int row, int column) const
	{
		Matrix<ROWS - 1, COLUMNS - 1> m;

		for (uint8_t i = 0, r = 0; i < ROWS; i++)
		{
			if (i != row)
			{
				for (uint8_t j = 0, c = 0; j < COLUMNS; j++)
				{
					if (j != column)
					{
						m.At(r, c) = this->At(i, j);
						c++;
					}
				}
				r++;
			}
		}

		return m;
	}

	float Matrix<ROWS, COLUMNS>::Determinant() const
	{
		static_assert((ROWS == COLUMNS), "The Matrix must be square i.e. have the same number of rows as columns.");

		float d = 0;
		if constexpr (ROWS == 2 && COLUMNS == 2)
		{
			d = this->At(0, 0) * this->At(1, 1) -
				this->At(0, 1) * this->At(1, 0);
		}
		else
		{
			for (uint8_t j = 0; j < COLUMNS; j++)
			{
				d += this->At(0, j) * this->Cofactor(0, j);
			}
		}
		return d;
	}

	float Minor(int row, int column) const
	{
		Matrix<ROWS - 1, COLUMNS - 1> submatrix = this->SubMatrix(row, column);

		return submatrix.Determinant();
	}

	float Cofactor(int row, int column) const
	{
		float minor = this->Minor(row, column);

		return (row + column) % 2 == 0 ? minor : -minor;
	}

	bool IsInvertible() const
	{
		return this->Determinant() != 0;
	}

	Matrix<ROWS, COLUMNS> Inverse() const
	{
		//static_assert(this->IsInvertible(), "The matrix must be invertible i.e. its determinant must be different from zero.");

		Matrix<ROWS, COLUMNS> m;

		for (uint8_t i = 0; i < ROWS; i++)
		{
			for (uint8_t j = 0; j < COLUMNS; j++)
			{
				// note that "col, row" here, instead of "row, col",
				// accomplishes the transpose operation!
				float c = this->Cofactor(i, j);
				m.At(j, i) = c / this->Determinant();
			}
		}

		return m;
	}

private:
	// the (0,0) point is at bottom-left!
	float data[ROWS][COLUMNS]{};
};

#endif