#pragma once

#include "pattern.h"

class RAYTRACER_EXPORT BinaryPattern : public Pattern
{
public:
	BinaryPattern() :
		Pattern(),
		pattern1(std::make_shared<FlatColor>(Color::White())),
		pattern2(std::make_shared<FlatColor>(Color::Black()))
	{}

	BinaryPattern(PatternPtr p1, PatternPtr p2,
		Matrix<4, 4> transformation) :
		Pattern(transformation),
		pattern1(p1),
		pattern2(p2)
	{}

	Color getColor1At(const Tuple& point) const
	{
		return this->pattern1->getColorAt(this->pattern1->getInverseTransformation() * point);
	}

	Color getColor2At(const Tuple& point) const
	{
		return this->pattern2->getColorAt(this->pattern2->getInverseTransformation() * point);
	}

	PatternPtr getPattern1() const { return pattern1; }
	PatternPtr getPattern2() const { return pattern2; }

	bool operator==(const BinaryPattern& other) const
	{
		return this->pattern1 == other.pattern1
			&& this->pattern2 == other.pattern2
			&& Pattern::operator==(other);
	}

protected:
	PatternPtr pattern1;
	PatternPtr pattern2;
};