//---------------------------------------------------------------------
// tuple.h Header
//---------------------------------------------------------------------
#ifndef TUPLE_H
#define TUPLE_H

#pragma once

#include <math.h>
#include <ostream>
#define MAX(a,b) a < b ? b : a

class __declspec(dllexport) Tuple
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

__declspec(dllexport) std::ostream& operator<<(std::ostream& os, Tuple const& v);
#endif
