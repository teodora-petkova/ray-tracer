#include "object.h"

IntersectionInfo Object::Intersect(const Ray& ray,
	std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const
{
	Ray transformedRay = ray * this->getInverseTransformation();

	// Transform the ray in object space by the inverse object transformation 
	// i.e. if the sphere is translated with 1 on x axis, the ray is translated in -1 on x axis
	// Transforming the ray has the effect of (potentially) stretching or shrinking its direction vector.
	// Leave the vector with its new length, unnormalized, so that when the distance is eventually computed,
	// it represents an intersection at the correct distance(in world space!) from the ray’s origin.
	// Remarks:
	// * Multiplying a point in object space by a transformation matrix converts that point to
	// world space—scaling it, translating, rotating it, or whatever.
	// * Multiplying a point in world space by the inverse of the transformation matrix 
	// converts that point back to object space.
	IntersectionParams intersection = LocalIntersect(transformedRay, intersectionDistances);

	// To find the intersection point we use the initial untransformed ray in the world space
	// to calculate locations in the world space using the calculated distance in the object space
	Tuple intersectionPoint = ray.getOrigin() + ray.getDirection() * intersection.getDistance();

	Tuple normal = this->getNormal(intersectionPoint, intersection);
	// check whether the ray comes from inside
	// the angle between the eye vector and the normal will be b/w 90 & 180
	// so the cos(angle) will be negative
	if (normal.Dot(-ray.getDirection()) < 0)
	{
		normal = -normal;
	}

	return IntersectionInfo(intersectionPoint,
		intersection.getDistance(),
		normal);
}

Tuple Object::TransformFromWorldToObjectSpace(Tuple point) const
{
	// transform a point from the world space to the local object space 
	// by the inverse transformation
	// NB! first try to transform from the world space to the parent object space

	Tuple objectPoint = point;

	if (this->parent != nullptr)
	{
		objectPoint = this->parent->TransformFromWorldToObjectSpace(point);
	}
	return this->getInverseTransformation() * objectPoint;
}

Tuple Object::TransformFromObjectToWorldSpace(Tuple vector) const
{
	// transform a vector from the local object space to the world space
	// by the transposed inverse transformation
	// NB! first try to transform from the object space to the outer space 
	// then if the object has a parent, convert from parent space to its outer space and etc.
	// till reaching the world space 

	Tuple worldVector = this->getTransposedInverseTransformation() * vector;
	worldVector = Tuple::Vector(worldVector.X(), worldVector.Y(), worldVector.Z());
	worldVector = worldVector.Normalize();

	if (this->parent != nullptr)
	{
		worldVector = this->parent->TransformFromObjectToWorldSpace(worldVector);
	}

	return worldVector;
}

Tuple Object::getNormal(Tuple intersectionPoint,
	const IntersectionParams& intersection) const
{
	Tuple objectPoint = TransformFromWorldToObjectSpace(intersectionPoint);

	Tuple objectNormal = getLocalNormal(objectPoint, intersection);

	Tuple worldNormal = TransformFromObjectToWorldSpace(objectNormal);

	return worldNormal;
}