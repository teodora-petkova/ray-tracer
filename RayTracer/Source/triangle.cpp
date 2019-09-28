//------------------------------------------------------------
// Triangle Class
//------------------------------------------------------------
#include "triangle.h"

using namespace std;

static bool SameSide(Vector3 p, Vector3 a, Vector3 b, Vector3 c)
{
	Vector3 vector1 = Cross(b - a, p - a);
	Vector3 vector2 = Cross(b - a, c - a);
	// in one direction
	if (Dot(vector1, vector2) >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}
static bool PointInTriangle(Vector3 p, Vector3 a, Vector3 b, Vector3 c)
{
	//If the point was on the same side of AB as C and
	//is also on the same side of BC as A and
	//on the same side of CA as B, then it is in the triangle.
	if (SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b))
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

IntersectionInfo Triangle::Intersect(Ray &ray)
{
	IntersectionInfo info = IntersectionInfo();
	float epsilon = 0.0000001f;

	Vector3    u, v, n;
	Vector3    w;
	float      t, a, b;

	// get triangle edge vectors and plane normal
	u = pointB - pointA; //AB = u
	v = pointC - pointA; //AC = v

	n = Cross(u, v);
	n.Normalize();

	//triangle is degenerate(a segment or a point)
	if (n == Vector3(0, 0, 0))
	{
		return info;
	}

	// ray: P = Po + P1.t
	// plane: P.n - A.n = 0;
	// t = (A.n - Po.n)/(P1.n);

	a = Dot((pointA - ray.GetOrigin()), n);
	b = Dot(ray.GetDirection(), n);

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
	t = a / b;
	// ray goes away from triangle => no intersect
	if (t < 0.0)
	{
		return info;
	}

	// for a segment, also test if (t > 1.0) => no intersect
	// find the intersection point of the ray and the plane
	Vector3 pointP = ray.GetOrigin() + ray.GetDirection() * t;

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
	float dot00 = Dot(v0, v0);
	float dot01 = Dot(v0, v1);
	float dot02 = Dot(v0, v2);
	float dot11 = Dot(v1, v1);
	float dot12 = Dot(v1, v2);

	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float uu = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float vv = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	if ((uu >= 0.0f) && (vv >= 0.0f) && (uu + vv < 1.0f))
	{
		return IntersectionInfo(true, t, n);
	}

	return info;
}

