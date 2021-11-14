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
	rayOrigin = origin;
	rayDirection = direction;
}

Ray Ray::operator*(const Matrix<4, 4>& m) const
{
	return Ray(m * this->getOrigin(), m * this->getDirection());
}