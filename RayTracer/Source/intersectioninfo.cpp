//---------------------------------------------------------------------
// intersectioninfo.cpp
//---------------------------------------------------------------------
#include "intersectioninfo.h"

IntersectionInfo::IntersectionInfo()
{
	this->isObjectHit = false;
	this->distance = 0.0;
	this->normal = Vector3();
}

IntersectionInfo::IntersectionInfo(bool isHit, float distance, Vector3 normal)
{
	this->isObjectHit = isHit;
	this->distance = distance;
	this->normal = normal;
}
