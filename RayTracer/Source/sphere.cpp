#include "sphere.h"

float Sphere::AlgebraicIntersect(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
{
	Tuple sphereCenterToRay = ray.getOrigin() - this->getCenter();

	float a = ray.getDirection().Dot(ray.getDirection());
	float b = 2 * ray.getDirection().Dot(sphereCenterToRay);
	float c = sphereCenterToRay.Dot(sphereCenterToRay) - this->getRadius() * this->getRadius();

	float discriminant = b * b - 4 * a * c;
	float distance = INFINITY;

	float root1 = 0.f;
	float root2 = 0.f;

	if (discriminant >= 0)
	{
		float d = sqrt(discriminant);
		float _2a = 2.0f * a;
		float root1 = (-b - d) / _2a;
		float root2 = (-b + d) / _2a;

		return getIntersectionResult(root1, root2, intersectionDistances);
	}

	return  distance;
}

float Sphere::GeometricIntersect(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
{
	float distance = INFINITY;

	Tuple l = this->getCenter() - ray.getOrigin();
	float t0 = l.Dot(ray.getDirection().Normalize());

	// there will be no intersection as the l vector and the ray vector will point
	// in opposite directions and even if there is an intersection, it will be behind the ray's origin
	// however, we need to collect all intersections even behind the ray's origin for the refraction!
	/*if (t0 < 0)
	{
		return distance;
	}*/

	float d2 = l.Dot(l) - t0 * t0;
	float r2 = this->getRadius() * this->getRadius();
	if (d2 > r2)
	{
		return distance;
	}

	float th0 = sqrt(r2 - d2);

	float magnitude = ray.getDirection().Magnitude();;

	float t1 = (t0 - th0) / magnitude;
	float t2 = (t0 + th0) / magnitude;

	return getIntersectionResult(t1, t2, intersectionDistances);
}

float Sphere::getIntersectionResult(const float& t1, const float& t2,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
{
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
		return INFINITY;
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