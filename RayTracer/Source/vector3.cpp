//---------------------------------------------------------------------
// Vector3.cpp
//---------------------------------------------------------------------
#include "vector3.h"

Vector3::Vector3()
{
	x = y = z = 0.0;
}
Vector3::Vector3(float f)
{
	x = f;
	y = f;
	z = f;
}
Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector3::Vector3(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
void Vector3::operator=(float f)
{
	x = f;
	y = f;
	z = f;
}
void Vector3::operator =(Vector3 v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
//------------------------------------------------------------------------
// Vector addition/subtraction/multiplication/division with another Vector
//------------------------------------------------------------------------
Vector3 Vector3::operator+(Vector3 v)
{
	return Vector3(v.x + x, v.y + y, v.z + z);
}
Vector3 Vector3::operator-(Vector3 v)
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}
Vector3 Vector3::operator*(Vector3 v)
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}
Vector3 Vector3::operator/(Vector3 v)
{
	return Vector3(x / v.x, y / v.y, z / v.z);
}
//------------------------------------------------------------------------
// Vector addition/subtraction/multiplication/division with a scalar.
//------------------------------------------------------------------------
Vector3 Vector3::operator+(float f)
{
	return Vector3(x + f, y + f, z + f);
}
Vector3 Vector3::operator-(float f)
{
	return Vector3(x - f, y - f, z - f);
}
Vector3 Vector3::operator*(float f)
{
	return Vector3(x * f, y * f, z * f);
}
Vector3 Vector3::operator/(float f)
{
	float recip;
	if (f < 0.000001f)  f = 1.0f;
	recip = 1.0f / f;
	return Vector3(x * recip, y * recip, z * recip);
}

Vector3 Vector3::operator-()
{
	return Vector3(-x, -y, -z);
}
//------------------------------------------------------------------------
// Vector addition/subtraction/multiplication/division with another Vector
//------------------------------------------------------------------------
void Vector3::operator +=(Vector3 v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}
void Vector3::operator -=(Vector3 v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}
void Vector3::operator *=(Vector3 v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
}
void Vector3::operator /=(Vector3 v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
}
//------------------------------------------------------------------------
// Vector addition/subtraction/multiplication/division with a scalar.
//------------------------------------------------------------------------
Vector3 Vector3::operator +=(float f)
{
	return Vector3(x + f, y + f, z + f);
}
Vector3 Vector3::operator -=(float f)
{
	return Vector3(x - f, y - f, z - f);
}
Vector3 Vector3::operator *=(float f)
{
	return Vector3(x * f, y * f, z * f);
}
Vector3 Vector3::operator /=(float f)
{
	float recip;
	if (f < 0.000001f)  f = 1.0f;
	recip = 1.0f / f;
	return Vector3(x * recip, y * recip, z * recip);
}

//------------------------------------------------------------------------
// Magnitude(), Dot(), Cross(), and Normalize() Functions
//------------------------------------------------------------------------
float Vector3::magnitude()
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

//v(x1, y1, z1) dot w(x2, y2, z2) = x1*x2 + y1*y2 + z1*z2
float dot(Vector3 v1, Vector3 v2)
{
	return(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

Vector3 cross(Vector3 v1, Vector3 v2)
{
	Vector3 v = Vector3();
	v.x = ((v1.y * v2.z) - (v1.z * v2.y));
	v.y = ((v1.z * v2.x) - (v1.x * v2.z));
	v.z = ((v1.x * v2.y) - (v1.y * v2.x));

	return v;
}

void Vector3::normalize()
{
	float m = magnitude();
	if (m < 0.0000001f)
	{
		m = 1.0f;
	}
	float invertedMagnitude = 1 / m;
	x = x * invertedMagnitude;
	y = y * invertedMagnitude;
	z = z * invertedMagnitude;
}

bool Vector3::operator==(const Vector3& v2) const
{
	return x == v2.x && y == v2.y && z == v2.z;
}