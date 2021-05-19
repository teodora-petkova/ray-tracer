//---------------------------------------------------------------------
// color.h Header
//---------------------------------------------------------------------
#ifndef COLOR_H
#define COLOR_H

#pragma once

#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include "tuple.h"
#include "raytracer_exports.h"

class RAYTRACER_EXPORT Color : public Tuple
{
public:
	Color() : Tuple() {}
	Color(Tuple const& t) : Tuple(t.x, t.y, t.z, t.w) {}
	Color(float r, float g, float b) : Tuple(r, g, b, 0.0f) {}

	float r() const { return this->x; }
	float g() const { return this->y; }
	float b() const { return this->z; }
};

#endif