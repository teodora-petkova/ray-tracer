#pragma once

#include "binarypattern.h"

class RAYTRACER_EXPORT RingPattern : public BinaryPattern
{
public:
	RingPattern() :
		BinaryPattern()
	{}

	RingPattern(PatternPtr p1, PatternPtr p2,
		Matrix<4, 4> transformation) :
		BinaryPattern(p1, p2, transformation)
	{}

	Color getColorAt(const Tuple& point) const
	{
		float x2 = point.X() * point.X();
		float z2 = point.Z() * point.Z();
		if ((int)floor(sqrt(x2 + z2)) % 2 == 0)
		{
			return getColor1At(point);
		}
		else
		{
			return getColor2At(point);
		}
	}
};