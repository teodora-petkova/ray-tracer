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
	Ray(const Tuple& origin, const Tuple& direction);

	Tuple getOrigin() const { return rayOrigin; }
	Tuple getDirection() const { return rayDirection; }

private:
	Tuple rayOrigin;
	Tuple rayDirection;
};

#endif