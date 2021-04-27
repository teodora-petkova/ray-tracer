//------------------------------------------------------------
// Cray.h Header
//------------------------------------------------------------
#ifndef RAY_H
#define RAY_H

#include "raytracer_exports.h"
#include "tuple.h"

class RAYTRACER_EXPORT Ray
{
public:
	Ray();

	Ray(Tuple origin, Tuple direction);

	Tuple getOrigin()
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