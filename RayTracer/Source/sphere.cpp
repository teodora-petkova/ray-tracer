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
int Sphere::Intersect(Ray &ray, float &distance)
{
	Vector3 originMinusSphereCenter = ray.GetOrigin() - sphereCenter;
	float a = Dot(ray.GetDirection(), ray.GetDirection());
	float b = 2 * Dot(ray.GetDirection(), originMinusSphereCenter);
	float c = Dot(originMinusSphereCenter, originMinusSphereCenter) - sphereRadius * sphereRadius;

	float discriminant = b * b - 4 * a * c;

	int return_value = 0;
	// If the discriminant is less than 0, then we totally miss the sphere.
	// This happens most of the time.
	if (discriminant > 0)
	{
		float d = sqrt(discriminant);
		double _2a = 2.0 * a;
		float root2 = (-b + d) / _2a;
		float root1 = (-b - d) / _2a;

		// If root2 < 0 and root1 < 0, so they are both misses.
		if (root2 > 0)
		{
			// If root2 > 0 and root1 < 0, we are inside the sphere.
			if (root1 < 0)
			{
				if (root2 < distance)
				{
					distance = root2;
					return_value = -1;
				}
			}
			// If root2 > 0 and root1 > 0, we hit the sphere.
			else
			{
				if (root1 < distance)
				{
					distance = root1;
					return_value = 1;
				}
			}
		}
	}
	return return_value;
}