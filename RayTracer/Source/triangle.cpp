//------------------------------------------------------------
// Triangle Class
//------------------------------------------------------------
#include "triangle.h"

using namespace std;

static bool isOnSameSide(Vector3 p, Vector3 a, Vector3 b, Vector3 c)
{
	Vector3 vector1 = Vector3::cross(b - a, p - a);
	Vector3 vector2 = Vector3::cross(b - a, c - a);
	// in one direction
	if (Vector3::dot(vector1, vector2) >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}

static bool isPointInTriangle(Vector3 p, Vector3 a, Vector3 b, Vector3 c)
{
	//IF the point P is:
	//	- on the same side of AB as C AND
	//  - on the same side of BC as A AND
	//  - on the same side of CA as B, 
	// THEN it is in the triangle.
	if (isOnSameSide(p, a, b, c) &&
		isOnSameSide(p, b, a, c) &&
		isOnSameSide(p, c, a, b))
	{
		return true;
	}
	else
	{
		return false;
	}
}

Triangle::Triangle()
{
	pointA = Vector3(-1.0, 0.0, 0.0);
	pointB = Vector3(1.0, 0.0, 0.0);
	pointC = Vector3(0.0, 1.0, 0.0);
}

IntersectionInfo Triangle::intersect(Ray& ray)
{
	IntersectionInfo info = IntersectionInfo();
	float epsilon = 0.0000001f;

	// get triangle edge vectors and plane normal
	Vector3 u = pointB - pointA; //AB = u
	Vector3 v = pointC - pointA; //AC = v

	Vector3 n = Vector3::cross(v, u).normalize();
	n.normalize();

	//triangle is degenerate(a segment or a point)
	if (n == Vector3(0, 0, 0))
	{
		return info;
	}

	// ray: P = Po + P1.t
	// plane: P.n - A.n = 0;
	// t = (A.n - Po.n)/(P1.n);

	float a = Vector3::dot((pointA - ray.getOrigin()), n);
	float b = Vector3::dot(ray.getDirection(), n);

	// ray is  parallel to triangle plane
	if (fabs(b) < epsilon)
	{
		//the ray lies in the triangle plane
		if (a == 0)
		{
			return info;
		}
		//ray - disjoint from plane
		else
		{
			return info;
		}
	}

	// get intersection point of ray with triangle plane
	float t = a / b;
	// ray goes away from triangle => no intersect
	if (t < 0.0)
	{
		return info;
	}

	// for a segment, also test if (t > 1.0) => no intersect
	// find the intersection point of the ray and the plane
	Vector3 pointP = ray.getOrigin() + ray.getDirection() * t;

	/*if(PointInTriangle(pointP, pointA, pointB, pointC))
	{
		return true;
	}
	return false;*/

	// Compute vectors        
	Vector3 v0 = pointC - pointA;
	Vector3 v1 = pointB - pointA;
	Vector3 v2 = pointP - pointA;

	// Compute dot products
	float dot00 = Vector3::dot(v0, v0);
	float dot01 = Vector3::dot(v0, v1);
	float dot02 = Vector3::dot(v0, v2);
	float dot11 = Vector3::dot(v1, v1);
	float dot12 = Vector3::dot(v1, v2);

	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float uu = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float vv = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	if ((uu >= 0.0f) && (vv >= 0.0f) && (uu + vv < 1.0f))
	{
		return IntersectionInfo(true, pointP, t, n);
	}

	return info;
}