#pragma once

#include "binarypattern.h"

class RAYTRACER_EXPORT CheckerPattern : public BinaryPattern
{
public:
	CheckerPattern() :
		BinaryPattern()
	{}

	CheckerPattern(const Color& c1, const Color& c2,
		Matrix<4, 4> transformation) :
		BinaryPattern(c1, c2, transformation)
	{}

	Color getColorAt(const Tuple& point) const
	{
		int sum = (int)floor(point.X()) + (int)floor(point.Y()) + (int)floor(point.Z());
		if (sum % 2 == 0)
		{
			return this->color1;
		}
		else
		{
			return this->color2;
		}
	}
};