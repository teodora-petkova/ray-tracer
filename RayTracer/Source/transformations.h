#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#pragma once

#pragma warning(push, 0)
#include <tuple>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "matrix.h"

namespace Transformations
{
	RAYTRACER_EXPORT Matrix<4, 4> Translation(float x, float y, float z);
	RAYTRACER_EXPORT Matrix<4, 4> Scaling(float x, float y, float z);
	RAYTRACER_EXPORT Matrix<4, 4> RotationX(float angleInDegrees);
	RAYTRACER_EXPORT Matrix<4, 4> RotationY(float angleInDegrees);
	RAYTRACER_EXPORT Matrix<4, 4> RotationZ(float angleInDegrees);
	RAYTRACER_EXPORT Matrix<4, 4> Shearing(float a, float b, float c, float d, float e, float f);
	RAYTRACER_EXPORT Matrix<4, 4> ViewTransform(Tuple from, Tuple to, Tuple up);
	RAYTRACER_EXPORT Matrix<4, 4> PerspectiveMatrix(float d, float apsectRatio, float near, float far);
};

#endif