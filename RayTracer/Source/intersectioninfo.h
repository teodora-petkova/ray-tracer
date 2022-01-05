#pragma once

#pragma warning(push, 0)
#include <vector>
#pragma warning(pop)

#include "tuple.h"

struct IntersectionInfo
{
public:
	IntersectionInfo() :
		isObjectHit(false),
		intersectionPoint(Tuple()),
		distance(0),
		normal(Tuple()),
		overPoint(Tuple())
	{}

	IntersectionInfo(bool isHit,
		const Tuple& intersectionPoint,
		float distance,
		const Tuple& normal) :
		isObjectHit(isHit),
		intersectionPoint(intersectionPoint),
		distance(distance),
		normal(normal),
		overPoint(intersectionPoint + normal * EPSILON),
		underPoint(intersectionPoint - normal * EPSILON)
	{}

	bool getIsHit() const { return isObjectHit; }
	Tuple getIntersectionPoint() const { return intersectionPoint; }
	Tuple getOverPoint() const { return overPoint; }
	Tuple getUnderPoint() const { return underPoint; }
	float getDistance() const { return distance; }
	Tuple getNormal() const { return normal; }

private:
	bool isObjectHit;
	Tuple intersectionPoint;
	Tuple overPoint;
	Tuple underPoint;
	float distance;
	Tuple normal;
};