//---------------------------------------------------------------------
// tuple.h Header
//---------------------------------------------------------------------
#ifndef TUPLE_H
#define TUPLE_H

#pragma once

#pragma warning(push, 0)
#include <cmath>
#include <ostream>
#pragma warning(pop)

#include "raytracer_exports.h"

#define MAX(a,b) a < b ? b : a

class RAYTRACER_EXPORT Tuple
{
public:
	Tuple();
	Tuple(float x, float y, float z, float w);
	static Tuple Point(float x, float y, float z);
	static Tuple Vector(float x, float y, float z);
	Tuple(const Tuple& v);
	void operator=(float f);
	void operator=(Tuple v);
	Tuple operator+(Tuple v);
	Tuple operator-(Tuple v);
	Tuple operator*(Tuple v);
	Tuple operator/(Tuple v);
	Tuple operator+(float f);
	Tuple operator-(float f);
	Tuple operator-();
	Tuple operator*(float f);
	Tuple operator/(float f);
	void operator+=(Tuple v);
	void operator-=(Tuple v);
	void operator*=(Tuple v);
	void operator/=(Tuple v);
	bool operator==(const Tuple& v2) const;
	float magnitude();
	Tuple normalize();

	static float dot(Tuple v1, Tuple v2);
	static Tuple cross(Tuple v1, Tuple v2);
	Tuple reflect(Tuple normal);

	bool isPoint();
	bool isVector();

	float x, y, z, w;
};

RAYTRACER_EXPORT std::ostream& operator<<(std::ostream& os, Tuple const& v);
#endif
