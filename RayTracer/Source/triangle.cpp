#include "triangle.h"

void Triangle::Initialize()
{
	// get triangle edge vectors and plane normal
	// the triangle's precomputed normal is used
	// for every point on the triangle

	this->AB = B - A;
	this->AC = C - A;
	this->BC = C - B;

	// left-handed coordinate system => -z is toward us
	this->normal = AC.Cross(AB).Normalize();

	this->bounds = BoundingBox();
	this->bounds.AddPoint(A);
	this->bounds.AddPoint(B);
	this->bounds.AddPoint(C);

	this->boundsInParentSpace = this->bounds;
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

	this->boundsInParentSpace = this->bounds.Transform(transformation);
}

Tuple Triangle::getLocalNormal(const Tuple& /*point*/, const IntersectionParams& /*intersection*/)const
{
	return this->normal;
}

bool Triangle::IsPointInTriangleByBarycentricCoordinates(const Tuple& P) const
{
	// compute dot products in advance
	Tuple AP = P - A;

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
	if ((AB).Cross(P - A).Dot(normal) >= 0 &&
		(BC).Cross(P - B).Dot(normal) >= 0 &&
		(-AC).Cross(P - C).Dot(normal) >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Triangle::LocalIntersect2(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances)const
{
	// the triangle is degenerate(a segment or a point)
	if (this->normal == Tuple::Vector(0, 0, 0))
	{
		return INFINITY;
	}

	// ray: P = Po + t.v
	// (Po - origin point, v - direction vector, P - intersection point)
	// plane: l*n = 0
	// (l = AP - any vector on the plane, n - the normal vector of the plane)
	// t = (PoA*n)/(v*n)

	float a = (A - ray.getOrigin()).Dot(this->normal);
	float b = (ray.getDirection()).Dot(this->normal);

	// ray is parallel to triangle plane
	if (isCloseToZero(b))
	{
		// if (a == 0) then the ray lies in the triangle plane
		// otherwise disjoint from the plane
		return INFINITY;
	}

	// get intersection point of ray with triangle plane
	float t = a / b;
	// ray goes away from triangle => no intersect
	if (t < 0.0) // TODO: ??? t>1.0 is it possible
	{
		return INFINITY;
	}

	// the intersection point of the ray and the plane
	Tuple P = ray.getOrigin() + ray.getDirection() * t;

	if (Triangle::IsPointInTriangleByBarycentricCoordinates(P))
		//if (Triangle::isPointInTriangleByHalfPlanes(P))
	{
		intersectionDistances.emplace_back(std::make_pair(t, shared_from_this()));
		return t;
	}

	return INFINITY;
}

/*
//Möller–Trumbore algorithm for ray-triangle intersection
// a point in barycentric coordinates inside a triangle
T(u, v) = (1-u-v).A + u.B + v.C

// a ray
R(t) = O + t.D (O - the origin point, D - the direction vector)

// the intersection
T(u, v) = R(t)

(1-u-v).A + u.B + v.C = O + t.D
=>
[(-D) (B-A) (C-A)] |t| = O - A
				   |u|
				   |v|

[(-D) AB AC] |t| = AO
			 |u|
			 |v|
// Cramer's Rule:

det =  [-D AB AC] = AB.(D x AC) // scalar triple product

t = (1 / det) * [AO AB AC] = (1 / det) * AC.(AO x AB)
u = (1 / det) * [-D AO AC] = (1 / det) * AO.( D x AC)
v = (1 / det) * [-D AB AO] = (1 / det) * D .(AO x AB)

*/
IntersectionParams Triangle::LocalIntersect(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
{
	// triple product = determinant!
	Tuple dirCrossAC = ray.getDirection().Cross(this->AC);
	float determinant = this->AB.Dot(dirCrossAC);
	if (abs(determinant) < EPSILON)
	{
		return INFINITY;
	}

	float f = 1.0 / determinant;

	Tuple AO = ray.getOrigin() - this->A;
	float u = f * AO.Dot(dirCrossAC);
	if (u < 0 || u > 1)
	{
		return INFINITY;
	}

	Tuple AOCrossAB = AO.Cross(this->AB);
	float v = f * ray.getDirection().Dot(AOCrossAB);
	if (v < 0 || (u + v) > 1)
	{
		return INFINITY;
	}

	float t = f * this->AC.Dot(AOCrossAB);
	intersectionDistances.emplace_back(std::make_pair(t, shared_from_this()));

	return IntersectionParams(t, u, v);
}