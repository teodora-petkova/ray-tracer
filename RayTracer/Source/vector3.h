//---------------------------------------------------------------------
// Vector3.h Header
//---------------------------------------------------------------------
#ifndef VECTOR3_H
#define VECTOR3_H

#pragma once

#include <math.h>
#define MAX(a,b) a < b ? b : a

class Vector3
{
public:
	Vector3();
	Vector3(float f);
	Vector3(float X, float Y, float Z);
	Vector3(const Vector3 &Vect);
	void operator=(float f);
	void operator=(Vector3 Vect);
	Vector3 operator +(Vector3 Vect);
	Vector3 operator -(Vector3 Vect);
	Vector3 operator *(Vector3 Vect);
	Vector3 operator /(Vector3 Vect);
	Vector3 operator +(float f);
	Vector3 operator -(float f);
	Vector3 operator -();
	Vector3 operator *(float f);
	Vector3 operator /(float f);
	void operator +=(Vector3 Vect);
	void operator -=(Vector3 Vect);
	void operator *=(Vector3 Vect);
	void operator /=(Vector3 Vect);
	Vector3 operator +=(float f);
	Vector3 operator -=(float f);
	Vector3 operator *=(float f);
	Vector3 operator /=(float f);
	bool operator ==(Vector3 Vect);
	float Magnitude();
	friend float Dot(Vector3 Vect1, Vector3 Vect2);
	void Normalize();
	friend Vector3 Cross(Vector3 Vect1, Vector3 Vect2);
	void DivideVectorByScaler(Vector3 Vector1, float f);

	float x, y, z;
};
#endif
