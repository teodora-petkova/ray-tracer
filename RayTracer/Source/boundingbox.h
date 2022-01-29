#pragma once

#pragma warning(push, 0)
#include <cmath>
#include <algorithm>
#pragma warning(pop)

#include "tuple.h"
#include "ray.h"
#include "matrix.h"

class RAYTRACER_EXPORT BoundingBox
{
public:
	BoundingBox() :
		min(Tuple::Point(INFINITY, INFINITY, INFINITY)),
		max(Tuple::Point(-INFINITY, -INFINITY, -INFINITY))
	{
		// An empty bounding box has 
		// its smallest point at positive infinity and
		// its largest point at negative infinity.
	}

	BoundingBox(const Tuple& min, const Tuple& max) :
		min(min),
		max(max)
	{}

	Tuple getMin() const { return min; }
	Tuple getMax() const { return max; }

	void AddPoint(Tuple p);
	void AddBound(BoundingBox b);

	bool Contains(const Tuple& point) const;
	bool Contains(const BoundingBox& b) const;

	BoundingBox Transform(const Matrix<4, 4>& matrix);

	bool Intersects(const Ray& ray, float& tmin, float& tmax) const;

private:
	Tuple min;
	Tuple max;
};

RAYTRACER_EXPORT std::ostream& operator<<(std::ostream& os, const BoundingBox& v);