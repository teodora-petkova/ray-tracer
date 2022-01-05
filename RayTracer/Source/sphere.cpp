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

float Sphere::GeometricIntersect(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
{
	Tuple l = this->getCenter() - ray.getOrigin();
	float t0 = l.Dot(ray.getDirection().Normalize());
	if (t0 < 0)
	{
		return  -1.f;
	}
	float d2 = l.Dot(l) - t0 * t0;
	float r2 = this->getRadius() * this->getRadius();
	if (d2 > r2)
	{
		return -1.f;
	}

	float th0 = sqrt(r2 - d2);

	float magnitude = ray.getDirection().Magnitude();;

	float t1 = (t0 - th0) / magnitude;
	float t2 = (t0 + th0) / magnitude;

	intersectionDistances.emplace_back(std::make_pair(t1, shared_from_this()));
	intersectionDistances.emplace_back(std::make_pair(t2, shared_from_this()));

	if (t1 >= 0 && t2 >= 0)
	{
		return ((t2 > t1) ? t1 : t2);
	}
	else if (t1 >= 0)
	{
		return t1;
	}
	else if (t2 >= 0)
	{
		return t2;
	}
	else
	{
		return -1;
	}
}

Tuple Sphere::getLocalNormal(const Tuple& intersectionPoint) const
{
	return (intersectionPoint - this->getCenter());
}

float Sphere::LocalIntersect(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
{
	return GeometricIntersect(ray, intersectionDistances);
}