#pragma once

#include "binarypattern.h"

class RAYTRACER_EXPORT Gradient : public BinaryPattern
{
public:
	Gradient() :
		BinaryPattern()
	{}

	Gradient(PatternPtr p1, PatternPtr p2,
		Matrix<4, 4> transformation) :
		BinaryPattern(p1, p2, transformation)
	{}

	Color getColorAt(const Tuple& point) const
	{
		Color color1 = getColor1At(point);
		Color color2 = getColor2At(point);

		Color distance = color2 - color1;

		float fraction = point.X() - floor(point.X());
		return color1 + distance * fraction;
	}
};