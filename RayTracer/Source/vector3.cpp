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
	x = f; y = f; z = f;
}
Vector3::Vector3(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
}
Vector3::Vector3(const Vector3 &Vect)
{
	x = Vect.x;
	y = Vect.y;
	z = Vect.z;
}
void Vector3::operator=(float f)
{
	x = f;
	y = f;
	z = f;
}
void Vector3::operator =(Vector3 Vect)
{
	x = Vect.x;
	y = Vect.y;
	z = Vect.z;
}
//------------------------------------------------------------------------
// Vector addition/subtraction/multiplication/division with another Vector
//------------------------------------------------------------------------
Vector3 Vector3::operator+(Vector3 Vect)
{
	return Vector3(Vect.x + x, Vect.y + y, Vect.z + z);
}
Vector3 Vector3::operator-(Vector3 Vect)
{
	return Vector3(x - Vect.x, y - Vect.y, z - Vect.z);
}
Vector3 Vector3::operator*(Vector3 Vect)
{
	return Vector3(x*Vect.x, y*Vect.y, z*Vect.z);
}
Vector3 Vector3::operator/(Vector3 Vect)
{
	return Vector3(x / Vect.x, y / Vect.y, z / Vect.z);
}
//------------------------------------------------------------------------
// Vector addition/subtraction/multiplication/division with a scalar.
//------------------------------------------------------------------------
Vector3 Vector3::operator +(float f)
{
	return Vector3(x + f, y + f, z + f);
}
Vector3 Vector3::operator -(float f)
{
	return Vector3(x - f, y - f, z - f);
}
Vector3 Vector3::operator *(float f)
{
	return Vector3(x*f, y*f, z*f);
}
Vector3 Vector3::operator /(float f)
{
	float recip;
	if (f < 0.000001f)  f = 1.0f;
	recip = 1.0f / f;
	return Vector3(x*recip, y*recip, z*recip);
}

Vector3 Vector3::operator-()
{
	return Vector3(-x, -y, -z);
}
//------------------------------------------------------------------------
// Vector addition/subtraction/multiplication/division with another Vector
//------------------------------------------------------------------------
void Vector3::operator +=(Vector3 Vect)
{
	x += Vect.x;
	y += Vect.y;
	z += Vect.z;
}
void Vector3::operator -=(Vector3 Vect)
{
	x -= Vect.x;
	y -= Vect.y;
	z -= Vect.z;
}
void Vector3::operator *=(Vector3 Vect)
{
	x *= Vect.x;
	y *= Vect.y;
	z *= Vect.z;
}
void Vector3::operator /=(Vector3 Vect)
{
	x /= Vect.x;
	y /= Vect.y;
	z /= Vect.z;
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
	return Vector3(x*f, y*f, z*f);
}
Vector3 Vector3::operator /=(float f)
{
	float recip;
	if (f < 0.000001f)  f = 1.0f;
	recip = 1.0f / f;
	return Vector3(x*recip, y*recip, z*recip);
}
//------------------------------------------------------------------------
// Magnitude(), Dot(), Cross(), and Normalize() Functions
//------------------------------------------------------------------------
float Vector3::Magnitude()
{
	return(float(sqrt((x*x) + (y*y) + (z*z))));
}

//v(x1, y1, z1) dot w(x2, y2, z2) = x1*x2 + y1*y2 + z1*z2
float Dot(Vector3 Vect1, Vector3 Vect2)
{
	return(Vect1.x*Vect2.x + Vect1.y*Vect2.y + Vect1.z*Vect2.z);
}

Vector3 Cross(Vector3 Vector1, Vector3 Vector2)
{
	Vector3 v = Vector3();
	v.x = ((Vector1.y*Vector2.z) - (Vector1.z*Vector2.y));
	v.y = ((Vector1.z*Vector2.x) - (Vector1.x*Vector2.z));
	v.z = ((Vector1.x*Vector2.y) - (Vector1.y*Vector2.x));

	return v;
}

void Vector3::Normalize()
{
	float magnitude = Magnitude();
	if (magnitude < 0.0000001f)
	{
		magnitude = 1.0f;
	}
	float invertedMagnitude = 1 / magnitude;
	x = x * invertedMagnitude;
	y = y * invertedMagnitude;
	z = z * invertedMagnitude;
}

void Vector3::DivideVectorByScaler(Vector3 Vector1, float f)
{
	x = Vector1.x / f;
	y = Vector1.y / f;
	z = Vector1.z / f;
}

bool Vector3::operator ==(Vector3 Vect)
{
	return x == Vect.x && y == Vect.y && z == Vect.z;
}