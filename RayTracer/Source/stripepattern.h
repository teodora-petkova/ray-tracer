#pragma once

#include "binarypattern.h"

class RAYTRACER_EXPORT StripePattern : public BinaryPattern
{
public:
	StripePattern() :
		BinaryPattern()
	{}

	StripePattern(PatternPtr p1, PatternPtr p2,
		Matrix<4, 4> transformation) :
		BinaryPattern(p1, p2, transformation)
	{}

	Color getColorAt(const Tuple& point) const
	{
		if (int(floor(point.X())) % 2 == 0)
		{
			return getColor1At(point);
		}
		else
		{
			return getColor2At(point);
		}
	}
};