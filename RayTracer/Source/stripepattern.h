//------------------------------------------------------------
// stripepattern.h Header
//------------------------------------------------------------
#ifndef STRIPEPATTERN_H
#define STRIPEPATTERN_H

#pragma once

#include "binarypattern.h"

class RAYTRACER_EXPORT StripePattern : public BinaryPattern
{
public:
	StripePattern() :
		BinaryPattern()
	{}

	StripePattern(const Color& c1, const Color& c2,
		Matrix<4, 4> transformation) :
		BinaryPattern(c1, c2, transformation)
	{}

	Color getColorAt(const Tuple& point) const
	{
		if (int(floor(point.X())) % 2 == 0)
		{
			return this->color1;
		}
		else
		{
			return this->color2;
		}
	}
};

#endif