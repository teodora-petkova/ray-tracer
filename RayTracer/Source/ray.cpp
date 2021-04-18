//------------------------------------------------------------
// Ray Class
//------------------------------------------------------------
#include "ray.h"

Ray::Ray()
{
	rayOrigin = Tuple::Point(0.0, 0.0, 0.0);
	rayDirection = Tuple::Vector(0.0, 0.0, 0.0);
}

Ray::Ray(const Tuple& origin, Tuple& direction)
{
	rayOrigin = origin;
	rayDirection = direction;
}