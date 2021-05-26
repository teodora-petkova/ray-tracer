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
	Tuple() : x(0), y(0), z(0), w(0)
	{}

	Tuple(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
	{}

	Tuple(const Tuple& v) : x(v.x), y(v.y), z(v.z), w(v.w)
	{}

	static Tuple Point(float x, float y, float z);
	static Tuple Vector(float x, float y, float z);

	bool operator==(const Tuple& v2) const;

	void operator=(float f);
	void operator=(const Tuple& v);

	Tuple operator+(const Tuple& v) const;
	Tuple operator-(const Tuple& v) const;
	Tuple operator*(const Tuple& v) const;
	Tuple operator/(const Tuple& v) const;

	Tuple operator-() const;
	Tuple operator+(float f) const;
	Tuple operator-(float f) const;
	Tuple operator*(float f) const;
	Tuple operator/(float f) const;

	void operator+=(const Tuple& v);
	void operator-=(const Tuple& v);
	void operator*=(const Tuple& v);
	void operator/=(const Tuple& v);

	float Magnitude() const;
	Tuple Normalize() const;

	float Dot(const Tuple& other) const;
	Tuple Cross(const Tuple& other) const;
	Tuple Reflect(const Tuple& normal) const;

	bool IsPoint() const;
	bool IsVector() const;

	float X() const { return x; }
	float Y() const { return y; }
	float Z() const { return z; }
	float W() const { return w; }

protected:
	float x, y, z, w;
};

RAYTRACER_EXPORT std::ostream& operator<<(std::ostream& os, const Tuple& v);
#endif
