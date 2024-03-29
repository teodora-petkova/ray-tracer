#pragma once

#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "types.h"
#include "color.h"
#include "matrix.h"

class RAYTRACER_EXPORT Pattern
{
public:
	Pattern()
		:invTransformation(Matrix<4, 4>::IdentityMatrix())
	{}

	Pattern(Matrix<4, 4> transform)
		:invTransformation(transform.Inverse())
	{}

	Matrix<4, 4> getInverseTransformation() const { return invTransformation; }

	virtual Color getColorAt(const Tuple& point) const = 0;
	Color getColorAtObject(ObjectConstPtr object, const Tuple& point) const;

	virtual bool operator==(const Pattern& other) const
	{
		return this->invTransformation == other.invTransformation;
	}

protected:
	Matrix<4, 4> invTransformation;
};