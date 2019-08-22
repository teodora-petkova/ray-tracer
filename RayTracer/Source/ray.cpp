//------------------------------------------------------------
// Ray Class
//------------------------------------------------------------
#include "ray.h"

Ray::Ray()
{
	rayOrigin = Vector3(0.0, 0.0, 0.0);
	rayDirection = Vector3(0.0, 0.0, 0.0);
}

Ray::Ray(const Vector3 &origin, Vector3 &direction)
{
	rayOrigin = origin;
	rayDirection = direction;
}