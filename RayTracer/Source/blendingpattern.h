#pragma once

#include "binarypattern.h"

class RAYTRACER_EXPORT BlendingPattern : public BinaryPattern
{
public:
	BlendingPattern() :
		BinaryPattern()
	{}

	BlendingPattern(PatternPtr p1, PatternPtr p2,
		Matrix<4, 4> transformation) :
		BinaryPattern(p1, p2, transformation)
	{}

	Color getColorAt(const Tuple& point) const
	{
		Color color1 = getColor1At(point);
		Color color2 = getColor2At(point);

		return (color1 + color2) / 2;
	}
};