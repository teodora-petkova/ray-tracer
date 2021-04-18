//---------------------------------------------------------------------
// Tuple.cpp
//---------------------------------------------------------------------
#include "tuple.h"

Tuple::Tuple()
{
	x = y = z = w = 0.0f;
}

Tuple::Tuple(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Tuple Tuple::Point(float x, float y, float z)
{
	return Tuple(x, y, z, 1.0f);
}

Tuple Tuple::Vector(float x, float y, float z)
{
	return Tuple(x, y, z, 0.0f);
}

bool Tuple::isPoint()
{
	return this->w == 1;
}

bool Tuple::isVector()
{
	return this->w == 0;
}

Tuple::Tuple(const Tuple& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

void Tuple::operator=(float f)
{
	x = f;
	y = f;
	z = f;
}

void Tuple::operator=(Tuple v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}
//------------------------------------------------------------------------
// Vector addition/subtraction/multiplication/division with another Vector
//------------------------------------------------------------------------

#define APPLY_OPERATOR_VECTOR(v, op) { x op v.x, y op v.y, z op v.z, w op v.w }
Tuple Tuple::operator+(Tuple v) { return APPLY_OPERATOR_VECTOR(v, +); }
Tuple Tuple::operator-(Tuple v) { return APPLY_OPERATOR_VECTOR(v, -); }
Tuple Tuple::operator*(Tuple v) { return APPLY_OPERATOR_VECTOR(v, *); }
Tuple Tuple::operator/(Tuple v) { return APPLY_OPERATOR_VECTOR(v, / ); }
#undef APPLY_OPERATOR_VECTOR

//------------------------------------------------------------------------
// Vector addition/subtraction/multiplication/division with a scalar.
//------------------------------------------------------------------------

#define APPLY_OPERATOR_SCALAR(s, op) { x op s, y op s, z op s, w op s }
Tuple Tuple::operator+(float s) { return APPLY_OPERATOR_SCALAR(s, +); }
Tuple Tuple::operator-(float s) { return APPLY_OPERATOR_SCALAR(s, -); }
Tuple Tuple::operator*(float s) { return APPLY_OPERATOR_SCALAR(s, *); }
Tuple Tuple::operator/(float s)
{
	float recip;
	if (s < 0.000001f)  s = 1.0f;
	recip = 1.0f / s;
	return APPLY_OPERATOR_SCALAR(recip, *);
}
#undef APPLY_OPERATOR_SCALAR

Tuple Tuple::operator-()
{
	return Tuple(-x, -y, -z, w);
}

//------------------------------------------------------------------------
// Vector addition/subtraction/multiplication/division with another Vector
//------------------------------------------------------------------------
#define APPLY_OPERATOR_INIT(v, op) { x op v.x; y op v.y; z op v.z; w op v.w; }
void Tuple::operator+=(Tuple v) { APPLY_OPERATOR_INIT(v, +=) }
void Tuple::operator-=(Tuple v) { APPLY_OPERATOR_INIT(v, -=) }
void Tuple::operator*=(Tuple v) { APPLY_OPERATOR_INIT(v, *=) }
void Tuple::operator/=(Tuple v) { APPLY_OPERATOR_INIT(v, /=) }
#undef APPLY_OPERATOR_INIT

//------------------------------------------------------------------------
// Magnitude(), Dot(), Cross(), and Normalize() Functions
//------------------------------------------------------------------------
float Tuple::magnitude()
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

//v(x1, y1, z1) dot w(x2, y2, z2) = x1*x2 + y1*y2 + z1*z2
float Tuple::dot(Tuple v1, Tuple v2)
{
	return(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

Tuple Tuple::cross(Tuple v1, Tuple v2)
{
	Tuple v = Tuple();
	v.x = ((v1.y * v2.z) - (v1.z * v2.y));
	v.y = ((v1.z * v2.x) - (v1.x * v2.z));
	v.z = ((v1.x * v2.y) - (v1.y * v2.x));
	v.w = v1.w;

	return v;
}

Tuple Tuple::normalize()
{
	float m = magnitude();
	if (m < 0.0000001f)
	{
		m = 1.0f;
	}
	float invertedMagnitude = 1 / m;
	return Tuple(
		x * invertedMagnitude,
		y * invertedMagnitude,
		z * invertedMagnitude,
		w);
}

bool Tuple::operator==(const Tuple& v2) const
{
	float epsilon = 0.0001f;
	return fabs(x - v2.x) < epsilon &&
		fabs(y - v2.y) < epsilon &&
		fabs(z - v2.z) < epsilon;
}

Tuple Tuple::reflect(Tuple normal)
{
	return *this - normal * dot(*this, normal) * 2;
}

std::ostream& operator<<(std::ostream& os, Tuple const& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return os;
}