//---------------------------------------------------------------------
// Vector3.h Header
//---------------------------------------------------------------------
#ifndef VECTOR3_H
#define VECTOR3_H

#pragma once

#include <math.h>
#define MAX(a,b) a < b ? b : a

class __declspec(dllexport) Vector3
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
	bool operator==(const Vector3& v2) const;
	float magnitude();
	friend float dot(Vector3 Vect1, Vector3 Vect2);
	void normalize();
	friend Vector3 cross(Vector3 Vect1, Vector3 Vect2);
	void divideVectorByScaler(Vector3 Vector1, float f);

	float x, y, z;
};
#endif
