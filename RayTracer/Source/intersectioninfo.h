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
	IntersectionInfo(bool isHit, float distance, Vector3 normal);

	bool IsHit()
	{
		return isHit;
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
	bool isHit;
	float distance;
	Vector3 normal;
};

#endif