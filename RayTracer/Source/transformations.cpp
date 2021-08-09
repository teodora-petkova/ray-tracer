#include "transformations.h"

Matrix<4, 4> Transformations::Translation(float x, float y, float z)
{
	return Matrix<4, 4>({
		{1, 0, 0, x},
		{0, 1, 0, y},
		{0, 0, 1, z},
		{0, 0, 0, 1} });
}

Matrix<4, 4> Transformations::Scaling(float x, float y, float z)
{
	return Matrix<4, 4>({
		{x, 0, 0, 0},
		{0, y, 0, 0},
		{0, 0, z, 0},
		{0, 0, 0, 1} });
}

Matrix<4, 4> Transformations::RotationX(float angle)
{
	return Matrix<4, 4>({
		{	1,	   		 0,			  0, 0},
		{	0,  cos(angle), -sin(angle), 0},
		{	0,	sin(angle),	 cos(angle), 0},
		{	0,			 0,	   		  0, 1} });
}

Matrix<4, 4> Transformations::RotationY(float angle)
{
	return Matrix<4, 4>({
		{	 cos(angle),  0, sin(angle), 0},
		{	          0,  1,          0, 0},
		{	-sin(angle),  0, cos(angle), 0},
		{	          0,  0,  		  0, 1} });
}

Matrix<4, 4> Transformations::RotationZ(float angle)
{
	return Matrix<4, 4>({
		{	cos(angle),	 -sin(angle), 0, 0},
		{	sin(angle),   cos(angle), 0, 0},
		{            0,	           0, 1, 0},
		{	         0,			   0, 0, 1} });
}

Matrix<4, 4> Transformations::Shearing(float a, float b, float c, float d, float e, float f)
{
	return Matrix<4, 4>({
		{1, a, b, 0},
		{c, 1, d, 0},
		{e, f, 1, 0},
		{0, 0, 0, 1} });
}