//------------------------------------------------------------
// Sphere Class
//------------------------------------------------------------
#include "sphere.h"

static std::pair<bool, float> AlgebraicIntersect(const Sphere& sphere, const Ray& ray)
{
	Tuple sphereCenterToRay = ray.getOrigin() - sphere.getCenter();
	float a = ray.getDirection().Dot(ray.getDirection());
	float b = 2 * ray.getDirection().Dot(sphereCenterToRay);
	float c = sphereCenterToRay.Dot(sphereCenterToRay) - sphere.getRadius() * sphere.getRadius();

	float discriminant = b * b - 4 * a * c;
	bool isHit = false;
	float distance = INFINITY;

	if (discriminant >= 0)
	{
		float d = sqrt(discriminant);
		float _2a = 2.0f * a;
		float root1 = (-b - d) / _2a;
		float root2 = (-b + d) / _2a;

		isHit = false;

		if (root1 > 0 || root2 > 0)
		{
			distance = (root1 < root2) ? root1 : root2;
			isHit = true;
		}
	}

	return std::make_pair(isHit, distance);
}

static std::pair<bool, float> GeometricIntersect(const Sphere& sphere, const Ray& ray)
{
	bool isHit = true;
	float distance = INFINITY;

	Tuple l = sphere.getCenter() - ray.getOrigin();
	float t0 = l.Dot(ray.getDirection().Normalize());
	if (t0 < 0)
	{
		return std::make_pair(false, distance);
	}
	float d2 = l.Dot(l) - t0 * t0;
	float r2 = sphere.getRadius() * sphere.getRadius();
	if (d2 > r2)
	{
		return std::make_pair(false, distance);
	}

	float th0 = sqrt(r2 - d2);

	float t1 = t0 - th0;
	float t2 = t0 + th0;

	distance = ((t2 > t1) ? t1 : t2) / ray.getDirection().Magnitude();

	return std::make_pair(isHit, distance);
}

Tuple Sphere::getLocalNormal(const Tuple& intersectionPoint) const
{
	return (intersectionPoint - this->getCenter());
}

std::pair<bool, float> Sphere::LocalIntersect(const Ray& ray) const
{
	return GeometricIntersect(*this, ray);
}