//---------------------------------------------------------------------
// intersectioninfo.h Header
//---------------------------------------------------------------------
#ifndef INTERSECTIONINFO_H
#define INTERSECTIONINFO_H

#include "tuple.h"

class IntersectionInfo
{
public:
	IntersectionInfo();
	IntersectionInfo(bool isHit, Tuple intersectionPoint, float distance, Tuple normal);

	bool isHit()
	{
		return isObjectHit;
	}

	Tuple getIntersectionPoint()
	{
		return intersectionPoint;
	}

	float getDistance()
	{
		return distance;
	}

	Tuple getNormal()
	{
		return normal;
	}

private:
	bool isObjectHit;
	Tuple intersectionPoint;
	float distance;
	Tuple normal;
};

#endif