#pragma once

#include "pattern.h"

class RAYTRACER_EXPORT BinaryPattern : public Pattern
{
public:
	BinaryPattern() :
		Pattern(),
		color1(Color::White()),
		color2(Color::Black())
	{}

	BinaryPattern(const Color& c1, const Color& c2,
		Matrix<4, 4> transformation) :
		Pattern(transformation),
		color1(c1),
		color2(c2)
	{}

	Color getColor1() const { return color1; }
	Color getColor2() const { return color2; }

protected:
	Color color1;
	Color color2;
};