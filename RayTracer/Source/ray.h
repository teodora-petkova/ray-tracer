#pragma once

#include "raytracer_exports.h"
#include "tuple.h"
#include "matrix.h"

class RAYTRACER_EXPORT Ray
{
public:
	Ray();
	Ray(const Tuple& origin, const Tuple& direction);

	Tuple getOrigin() const { return rayOrigin; }
	Tuple getDirection() const { return rayDirection; }

	Ray operator*(const Matrix<4, 4>& m) const;

private:
	Tuple rayOrigin;
	Tuple rayDirection;
};