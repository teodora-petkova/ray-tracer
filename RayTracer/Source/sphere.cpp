//------------------------------------------------------------
// Sphere Class
//------------------------------------------------------------
#include "sphere.h"

using namespace std;

Sphere::Sphere()
{
	sphereCenter = Vector3(0.0, 0.0, 0.0);
	sphereRadius = 1.0;
	sphereRadiusSquared = 1.0;
	sphereRadiusInverse = 1.0;
}

IntersectionInfo Sphere::Intersect(Ray &ray)
{
	Vector3 originMinusSphereCenter = ray.GetOrigin() - sphereCenter;
	float a = Dot(ray.GetDirection(), ray.GetDirection());
	float b = 2 * Dot(ray.GetDirection(), originMinusSphereCenter);
	float c = Dot(originMinusSphereCenter, originMinusSphereCenter) - sphereRadius * sphereRadius;

	float discriminant = b * b - 4 * a * c;
	bool isHit = false;
	float distance = INFINITY;
	// If the discriminant is less than 0, then we totally miss the sphere.
	// This happens most of the time.
	if (discriminant > 0)
	{
		float d = sqrt(discriminant);
		float _2a = 2.0f * a;
		float root1 = (-b - d) / _2a;
		float root2 = (-b + d) / _2a;

		isHit = false;
		// If root2 < 0 and root1 < 0, so they are both misses.
		if (root1 > 0 && root2 > 0)
		{
			// If root2 > 0 and root1 > 0, we hit the sphere.
			if (root1 < root2)
			{
				distance = root1;
			}
			else
			{
				distance = root2;
			}
			isHit = true;
		}
		// If root2 > 0 and root1 < 0, we are inside the sphere.
		else if (root1 < 0 && root2 > 0)
		{
			distance = root2;
			//isHit = true;
		}
		else if (root1 > 0 && root2 < 0)
		{
			distance = root1;
			//isHit = true;
		}
	}
	Vector3 intersectionPoint = ray.GetOrigin() + ray.GetDirection() * distance;
	Vector3 normal = (intersectionPoint - sphereCenter)*distance;
	normal.Normalize();

	return IntersectionInfo(isHit, distance, normal);
}