//---------------------------------------------------------------------
// intersectioninfo.h Header
//---------------------------------------------------------------------
#ifndef INTERSECTIONINFO_H
#define INTERSECTIONINFO_H

#include "vector3.h"

class IntersectionInfo
{
public:
	IntersectionInfo();
	IntersectionInfo(bool isHit, Vector3 intersectionPoint, float distance, Vector3 normal);

	bool isHit()
	{
		return isObjectHit;
	}

	Vector3 getIntersectionPoint()
	{
		return intersectionPoint;
	}

	float getDistance()
	{
		return distance;
	}

	Vector3 getNormal()
	{
		return normal;
	}

private:
	bool isObjectHit;
	Vector3 intersectionPoint;
	float distance;
	Vector3 normal;
};

#endif