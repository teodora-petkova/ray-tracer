//------------------------------------------------------------
// Triangle Class
//------------------------------------------------------------
#include "triangle.h"

void Triangle::Initialize()
{
	// get triangle edge vectors and plane normal
	// the triangle's precomputed normal is used
	// for every point on the triangle

	Tuple AB = B - A;
	Tuple AC = C - A;

	// right-handed coordinate system!!!
	normal = AB.Cross(AC).Normalize();
}

Tuple Triangle::getLocalNormal(const Tuple& point)const
{
	return this->normal;
}

bool Triangle::IsPointInTriangleByBarycentricCoordinates(const Tuple& P) const
{
	// compute dot products in advance
	Tuple AP = P - A;
	Tuple AB = B - A;
	Tuple AC = C - A;

	float d00 = AB.Dot(AB);
	float d01 = AB.Dot(AC);
	float d02 = AB.Dot(AP);
	float d11 = AC.Dot(AC);
	float d12 = AC.Dot(AP);

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

bool Triangle::IsPointInTriangleByHalfPlanes(const Tuple& P) const
{
	if ((B - A).Cross(P - A).Dot(normal) >= 0 &&
		(C - B).Cross(P - B).Dot(normal) >= 0 &&
		(A - C).Cross(P - C).Dot(normal) >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Triangle::Triangle() :
	Object(),
	A(Tuple::Vector(-1.0, 0.0, 0.0)),
	B(Tuple::Vector(1.0, 0.0, 0.0)),
	C(Tuple::Vector(0.0, 1.0, 0.0))
{
	Initialize();
}

Triangle::Triangle(const Tuple& point1, const Tuple& point2, const Tuple& point3,
	MaterialPtr material, Matrix<4, 4> transformation) :
	Object(material, transformation),
	A(point1),
	B(point2),
	C(point3)
{
	Initialize();
}

std::pair<bool, float> Triangle::LocalIntersect(const Ray& ray) const
{
	std::pair<bool, float> isHitWithDistance = std::make_pair(false, 0.0f);

	// the triangle is degenerate(a segment or a point)
	if (this->normal == Tuple::Vector(0, 0, 0))
	{
		return isHitWithDistance;
	}

	// ray: P = Po + t.v
	// (Po - origin point, v - direction vector, P - intersection point)
	// plane: l*n = 0
	// (l = AP - any vector on the plane, n - the normal vector of the plane)
	// t = (PoA*n)/(v*n)

	float a = (A - ray.getOrigin()).Dot(this->normal);
	float b = (ray.getDirection()).Dot(this->normal);

	// ray is  parallel to triangle plane
	if (isCloseToZero(b))
	{
		// if (a == 0) then the ray lies in the triangle plane
		// otherwise disjoint from the plane
		return isHitWithDistance;
	}

	// get intersection point of ray with triangle plane
	float t = a / b;
	// ray goes away from triangle => no intersect
	if (t < 0.0) // TODO: ??? t>1.0 is it possible
	{
		return isHitWithDistance;
	}

	// the intersection point of the ray and the plane
	Tuple P = ray.getOrigin() + ray.getDirection() * t;

	if (Triangle::IsPointInTriangleByBarycentricCoordinates(P))
		//if (Triangle::isPointInTriangleByHalfPlanes(P))
	{
		return std::make_pair(true, t);
	}

	return isHitWithDistance;
}