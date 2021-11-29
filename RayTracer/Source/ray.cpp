//------------------------------------------------------------
// Ray Class
//------------------------------------------------------------
#include "ray.h"

Ray::Ray()
{
	rayOrigin = Tuple::Point(0.0, 0.0, 0.0);
	rayDirection = Tuple::Vector(0.0, 0.0, 0.0);
}

Ray::Ray(const Tuple& origin, const Tuple& direction)
{
	rayOrigin = Tuple::Point(origin.X(), origin.Y(), origin.Z());
	rayDirection = Tuple::Vector(direction.X(), direction.Y(), direction.Z());
}

Ray Ray::operator*(const Matrix<4, 4>& m) const
{
	Tuple o = m * this->getOrigin();
	Tuple d = m * this->getDirection();
	return Ray(
		Tuple::Point(o.X(), o.Y(), o.Z()),
		Tuple::Vector(d.X(), d.Y(), d.Z()));
}