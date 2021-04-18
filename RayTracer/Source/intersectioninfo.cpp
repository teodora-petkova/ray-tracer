//---------------------------------------------------------------------
// intersectioninfo.cpp
//---------------------------------------------------------------------
#include "intersectioninfo.h"

IntersectionInfo::IntersectionInfo()
{
	this->isObjectHit = false;
	this->distance = 0.0;
	this->normal = Tuple();
}

IntersectionInfo::IntersectionInfo(bool isHit, Tuple intersectionPoint, float distance, Tuple normal)
{
	this->isObjectHit = isHit;
	this->intersectionPoint = intersectionPoint;
	this->distance = distance;
	this->normal = normal;
}
