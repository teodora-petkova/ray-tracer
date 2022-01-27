#pragma once

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

#include "tuple.h"

struct IntersectionParams
{
public:
	IntersectionParams() :
		distance(INFINITY),
		u(INFINITY),
		v(INFINITY)
	{}

	IntersectionParams(float distance) :
		distance(distance),
		u(INFINITY),
		v(INFINITY)
	{}

	IntersectionParams(float distance,
		float u, float v) :
		distance(distance),
		u(u),
		v(v)
	{}

	float getDistance() const { return distance; }
	float getU() const { return u; }
	float getV() const { return v; }

private:
	float distance;
	float u;
	float v;
};

struct IntersectionInfo
{
public:
	IntersectionInfo() :
		isObjectHit(false),
		intersectionPoint(Tuple()),
		object(nullptr),
		distance(INFINITY),
		u(INFINITY),
		v(INFINITY),
		normal(Tuple()),
		overPoint(Tuple())
	{}

	IntersectionInfo(const Tuple& intersectionPoint,
		IntersectionParams intersection,
		ObjectConstPtr obj,
		const Tuple& normal) :
		isObjectHit(intersection.getDistance() != INFINITY && intersection.getDistance() >= 0),
		intersectionPoint(intersectionPoint),
		object(obj),
		distance(intersection.getDistance()),
		u(intersection.getU()),
		v(intersection.getV()),
		normal(normal),
		overPoint(intersectionPoint + normal * EPSILON),
		underPoint(intersectionPoint - normal * EPSILON)
	{}

	bool getIsHit() const { return isObjectHit; }
	Tuple getIntersectionPoint() const { return intersectionPoint; }
	ObjectConstPtr getObject() const { return object; }
	Tuple getOverPoint() const { return overPoint; }
	Tuple getUnderPoint() const { return underPoint; }
	float getDistance() const { return distance; }
	float getU() const { return u; }
	float getV() const { return v; }
	Tuple getNormal() const { return normal; }

private:
	bool isObjectHit;
	Tuple intersectionPoint;
	ObjectConstPtr object;
	Tuple overPoint;
	Tuple underPoint;
	float distance;
	float u;
	float v;
	Tuple normal;
};