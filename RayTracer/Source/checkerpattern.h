#pragma once

#include "binarypattern.h"

class RAYTRACER_EXPORT CheckerPattern : public BinaryPattern
{
public:
	CheckerPattern() :
		BinaryPattern()
	{}

	CheckerPattern(PatternPtr p1, PatternPtr p2,
		Matrix<4, 4> transformation) :
		BinaryPattern(p1, p2, transformation)
	{}

	Color getColorAt(const Tuple& point) const
	{
		int sum =
			static_cast<int>(floor(point.X())) +
			static_cast<int>(floor(point.Y())) +
			static_cast<int>(floor(point.Z()));
		if (sum % 2 == 0)
		{
			return getColor1At(point);
		}
		else
		{
			return getColor2At(point);
		}
	}
};