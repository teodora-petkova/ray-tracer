#pragma once

#include "binarypattern.h"

class RAYTRACER_EXPORT RingPattern : public BinaryPattern
{
public:
	RingPattern() :
		BinaryPattern()
	{}

	RingPattern(const Color& c1, const Color& c2,
		Matrix<4, 4> transformation) :
		BinaryPattern(c1, c2, transformation)
	{}

	Color getColorAt(const Tuple& point) const
	{
		float x2 = point.X() * point.X();
		float z2 = point.Z() * point.Z();
		if ((int)floor(sqrt(x2 + z2)) % 2 == 0)
		{
			return this->color1;
		}
		else
		{
			return this->color2;
		}
	}
};