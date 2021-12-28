#pragma once

#include "binarypattern.h"

class RAYTRACER_EXPORT RadialGradient : public BinaryPattern
{
public:
	RadialGradient() :
		BinaryPattern()
	{}

	RadialGradient(PatternPtr p1, PatternPtr p2,
		Matrix<4, 4> transformation) :
		BinaryPattern(p1, p2, transformation)
	{}

	Color getColorAt(const Tuple& point) const
	{
		Color color1 = getColor1At(point);
		Color color2 = getColor2At(point);

		Color distance = color2 - color1;

		float x2 = point.X() * point.X();
		float z2 = point.Z() * point.Z();

		float fraction = sqrt(x2 + z2) - floor(sqrt(x2 + z2));
		return color1 + distance * fraction;
	}
};