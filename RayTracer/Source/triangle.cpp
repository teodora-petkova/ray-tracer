//------------------------------------------------------------
// Triangle Class
//------------------------------------------------------------
#include "triangle.h"

using namespace std;

static bool isOnSameSide(Tuple p, Tuple a, Tuple b, Tuple c)
{
	Tuple vector1 = Tuple::cross(b - a, p - a);
	Tuple vector2 = Tuple::cross(b - a, c - a);
	// in one direction
	if (Tuple::dot(vector1, vector2) >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}

static bool isPointInTriangle(Tuple p, Tuple a, Tuple b, Tuple c)
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
	pointA = Tuple::Vector(-1.0, 0.0, 0.0);
	pointB = Tuple::Vector(1.0, 0.0, 0.0);
	pointC = Tuple::Vector(0.0, 1.0, 0.0);
}

IntersectionInfo Triangle::intersect(Ray& ray)
{
	IntersectionInfo info = IntersectionInfo();
	float epsilon = 0.0000001f;

	// get triangle edge vectors and plane normal
	Tuple u = pointB - pointA; //AB = u
	Tuple v = pointC - pointA; //AC = v

	Tuple n = Tuple::cross(v, u).normalize();
	n.normalize();

	//triangle is degenerate(a segment or a point)
	if (n == Tuple::Vector(0, 0, 0))
	{
		return info;
	}

	// ray: P = Po + P1.t
	// plane: P.n - A.n = 0;
	// t = (A.n - Po.n)/(P1.n);

	float a = Tuple::dot((pointA - ray.getOrigin()), n);
	float b = Tuple::dot(ray.getDirection(), n);

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
	Tuple pointP = ray.getOrigin() + ray.getDirection() * t;

	/*if(PointInTriangle(pointP, pointA, pointB, pointC))
	{
		return true;
	}
	return false;*/

	// Compute vectors        
	Tuple v0 = pointC - pointA;
	Tuple v1 = pointB - pointA;
	Tuple v2 = pointP - pointA;

	// Compute dot products
	float dot00 = Tuple::dot(v0, v0);
	float dot01 = Tuple::dot(v0, v1);
	float dot02 = Tuple::dot(v0, v2);
	float dot11 = Tuple::dot(v1, v1);
	float dot12 = Tuple::dot(v1, v2);

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