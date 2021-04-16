//------------------------------------------------------------
// Cray.h Header
//------------------------------------------------------------
#ifndef CRAY_H
#define CRAY_H

#include "vector3.h"

class Ray
{
public:
	Ray();

	Ray(const Vector3& origin, Vector3& direction);

	Vector3& getOrigin()
	{
		return rayOrigin;
	}

	Vector3& getDirection()
	{
		return rayDirection;
	}

private:
	Vector3 rayOrigin;
	Vector3 rayDirection;
};

#endif