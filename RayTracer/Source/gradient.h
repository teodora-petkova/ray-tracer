#pragma once

#include "binarypattern.h"

class RAYTRACER_EXPORT Gradient : public BinaryPattern
{
public:
	Gradient() :
		BinaryPattern()
	{}

	Gradient(const Color& c1, const Color& c2,
		Matrix<4, 4> transformation) :
		BinaryPattern(c1, c2, transformation)
	{}

	Color getColorAt(const Tuple& point) const
	{
		Color distance = this->color2 - this->color1;
		float fraction = point.X() - floor(point.X());
		return this->color1 + distance * fraction;
	}
};