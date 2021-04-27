//------------------------------------------------------------
// Triangle Class
//------------------------------------------------------------
#include "triangle.h"

using namespace std;

void Triangle::initialize()
{
	// get triangle edge vectors and plane normal
	// the triangle's precomputed normal is used
	// for every point on the triangle

	Tuple AB = B - A;
	Tuple AC = C - A;

	// right-handed coordinate system!!!
	normal = Tuple::cross(AB, AC).normalize();
}

bool Triangle::isPointInTriangleByBarycentricCoordinates(Tuple P)
{
	// compute dot products in advance
	Tuple AP = P - A;
	Tuple AB = B - A;
	Tuple AC = C - A;

	float d00 = Tuple::dot(AB, AB);
	float d01 = Tuple::dot(AB, AC);
	float d02 = Tuple::dot(AB, AP);
	float d11 = Tuple::dot(AC, AC);
	float d12 = Tuple::dot(AC, AP);

	// compute barycentric coordinates
	float inverted_denominator = 1 / (d00 * d11 - d01 * d01);
	float beta = (d02 * d11 - d01 * d12) * inverted_denominator;
	float lambda = (d00 * d12 - d01 * d02) * inverted_denominator;

	// check if point is in triangle based on the barycentric coordinates
	// alpha = 1 - (beta + lambda)
	if ((beta >= 0.0f) && (lambda >= 0.0f) && (beta + lambda < 1.0f))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Triangle::isPointInTriangleByHalfPlanes(Tuple P)
{
	if (Tuple::dot(Tuple::cross(B - A, P - A), normal) >= 0 &&
		Tuple::dot(Tuple::cross(C - B, P - B), normal) >= 0 &&
		Tuple::dot(Tuple::cross(A - C, P - C), normal) >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Triangle::Triangle() :
	A(Tuple::Vector(-1.0, 0.0, 0.0)),
	B(Tuple::Vector(1.0, 0.0, 0.0)),
	C(Tuple::Vector(0.0, 1.0, 0.0))
{
	initialize();
}

Triangle::Triangle(Tuple& point1, Tuple& point2, Tuple& point3) :
	A(point1),
	B(point2),
	C(point3)
{
	initialize();
}

IntersectionInfo Triangle::intersect(Ray& ray)
{
	float epsilon = 0.0000001f;

	IntersectionInfo info = IntersectionInfo();

	// the triangle is degenerate(a segment or a point)
	if (this->normal == Tuple::Vector(0, 0, 0))
	{
		return info;
	}

	// ray: P = Po + t.v
	// (Po - origin point, v - direction vector, P - intersection point)
	// plane: l*n = 0
	// (l = AP - any vector on the plane, n - the normal vector of the plane)
	// t = (PoA*n)/(v*n)

	float a = Tuple::dot((A - ray.getOrigin()), this->normal);
	float b = Tuple::dot(ray.getDirection(), this->normal);

	// ray is  parallel to triangle plane
	if (fabs(b) < epsilon)
	{
		// if (a == 0) then the ray lies in the triangle plane
		// otherwise disjoint from the plane
		return info;
	}

	// get intersection point of ray with triangle plane
	float t = a / b;
	// ray goes away from triangle => no intersect
	if (t < 0.0) // TODO: ??? t>1.0 is it possible
	{
		return info;
	}

	// the intersection point of the ray and the plane
	Tuple P = ray.getOrigin() + ray.getDirection() * t;

	if (Triangle::isPointInTriangleByBarycentricCoordinates(P))
		//if (Triangle::isPointInTriangleByHalfPlanes(P))
	{
		return IntersectionInfo(true, P, t, this->normal);
	}

	return info;
}