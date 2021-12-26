#pragma once

#include "pattern.h"

class RAYTRACER_EXPORT FlatColor : public Pattern
{
public:
	FlatColor() :
		Pattern(),
		color(Color::White())
	{}

	FlatColor(const Color& c) :
		Pattern(Matrix<4, 4>::IdentityMatrix()),
		color(c)
	{}

	Color getColor() const { return color; }

	Color getColorAt(const Tuple& /*point*/) const
	{
		return this->color;
	}

private:
	Color color;
};