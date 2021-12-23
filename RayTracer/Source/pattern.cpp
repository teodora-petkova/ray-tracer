//---------------------------------------------------------------------
// pattern.cpp
//---------------------------------------------------------------------
#include "pattern.h"
#include "object.h"

Color Pattern::getColorAtObject(const ObjectPtr& object, const Tuple& point) const
{
	Tuple objectPoint = object->getInverseTransformation() * point;
	Tuple patternPoint = this->getInverseTransformation() * objectPoint;
	return getColorAt(patternPoint);
}