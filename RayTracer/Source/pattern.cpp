#include "pattern.h"
#include "object.h"

Color Pattern::getColorAtObject(ObjectConstPtr object, const Tuple& point) const
{
	Tuple objectPoint = object->TransformFromWorldToObjectSpace(point);
	Tuple patternPoint = this->getInverseTransformation() * objectPoint;
	return getColorAt(patternPoint);
}