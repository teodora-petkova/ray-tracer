#pragma once

#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include "tuple.h"
#include "raytracer_exports.h"

class RAYTRACER_EXPORT Color : public Tuple
{
public:
	Color() : Tuple(1.0f, 1.0f, 1.0f, 0.0f) {}
	Color(Tuple const& t) : Tuple(t.X(), t.Y(), t.Z(), t.W()) {}
	Color(float r, float g, float b) : Tuple(r, g, b, 0.0f) {}

	float R() const { return this->x; }
	float G() const { return this->y; }
	float B() const { return this->z; }

	static Color Black() { return Color(0.0f, 0.0f, 0.0f); }
	static Color White() { return Color(1.0f, 1.0f, 1.0f); }
};