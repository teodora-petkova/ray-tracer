#pragma once

#include "tuple.h"

class IntersectionInfo
{
public:
	IntersectionInfo() :
		isObjectHit(false),
		intersectionPoint(Tuple()),
		distance(0),
		normal(Tuple())
	{}

	IntersectionInfo(bool isHit,
		const Tuple& intersectionPoint,
		float distance, const Tuple& normal) :
		isObjectHit(isHit),
		intersectionPoint(intersectionPoint),
		distance(distance),
		normal(normal)
	{}

	bool getIsHit() const { return isObjectHit; }
	Tuple getIntersectionPoint() const { return intersectionPoint; }
	float getDistance() const { return distance; }
	Tuple getNormal() const { return normal; }

private:
	bool isObjectHit;
	Tuple intersectionPoint;
	float distance;
	Tuple normal;
};