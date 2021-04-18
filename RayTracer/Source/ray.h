//------------------------------------------------------------
// Cray.h Header
//------------------------------------------------------------
#ifndef CRAY_H
#define CRAY_H

#include "Tuple.h"

class Ray
{
public:
	Ray();

	Ray(const Tuple& origin, Tuple& direction);

	Tuple& getOrigin()
	{
		return rayOrigin;
	}

	Tuple& getDirection()
	{
		return rayDirection;
	}

private:
	Tuple rayOrigin;
	Tuple rayDirection;
};

#endif