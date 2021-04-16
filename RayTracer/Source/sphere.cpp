//------------------------------------------------------------
// Sphere Class
//------------------------------------------------------------
#include "sphere.h"

using namespace std;

Sphere::Sphere()
{
	center = Vector3(0.0, 0.0, 0.0);
	radius = 1.0;
}

IntersectionInfo Sphere::intersect(Ray& ray)
{
	Vector3 originMinusSphereCenter = ray.getOrigin() - center;
	float a = Vector3::dot(ray.getDirection(), ray.getDirection());
	float b = 2 * Vector3::dot(ray.getDirection(), originMinusSphereCenter);
	float c = Vector3::dot(originMinusSphereCenter, originMinusSphereCenter) - radius * radius;

	float discriminant = b * b - 4 * a * c;
	bool isHit = false;
	float distance = INFINITY;

	if (discriminant > 0)
	{
		float d = sqrt(discriminant);
		float _2a = 2.0f * a;
		float root1 = (-b - d) / _2a;
		float root2 = (-b + d) / _2a;

		isHit = false;
		if (root1 > 0 && root2 > 0)
		{
			distance = (root1 < root2) ? root1 : root2;
			isHit = true;
		}
	}
	Vector3 intersectionPoint = ray.getOrigin() + ray.getDirection() * distance;
	Vector3 normal = ((intersectionPoint - center) * distance).normalize();

	return IntersectionInfo(isHit, intersectionPoint, distance, normal);
}