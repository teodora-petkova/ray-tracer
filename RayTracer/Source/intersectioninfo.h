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
	float GetDistance()
	{
		return distance;
	}
	Vector3 GetNormal()
	{
		return normal;
	}

private:
	bool isHit;
	float distance;
	Vector3 normal;
};

#endif