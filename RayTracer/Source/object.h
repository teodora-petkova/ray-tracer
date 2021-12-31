#pragma once

#include "raytracer_exports.h"
#include "tuple.h"
#include "ray.h"
#include "material.h"
#include "intersectioninfo.h"

class RAYTRACER_EXPORT Object
{
public:
	Object() :
		material(std::make_shared<Material>()),
		invTransformation(Matrix<4, 4>::IdentityMatrix()),
		transposedInvTransformation(Matrix<4, 4>::IdentityMatrix())
	{}

	Object(MaterialPtr material, Matrix<4, 4> matrix) :
		material(material)
	{
		Matrix<4, 4> inverseTransformation = matrix.Inverse();
		this->invTransformation = inverseTransformation;
		this->transposedInvTransformation = inverseTransformation.Transpose();
	}

	IntersectionInfo Intersect(const Ray& ray) const
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
		auto pair = LocalIntersect(transformedRay);
		bool isHit = pair.first;
		float distance = pair.second;

		// To find the intersection point we use the initial untransformed ray in the world space
		// to calculate locations in the world space using the calculated distance in the object space
		Tuple intersectionPoint = ray.getOrigin() + ray.getDirection() * distance;

		Tuple normal = this->getNormal(intersectionPoint);
		// check whether the ray comes from inside
		// the angle between the eye vector and the normal will be b/w 90 & 180
		// so the cos(angle) will be negative
		if (normal.Dot(-ray.getDirection()) < 0)
		{
			normal = -normal;
		}

		return IntersectionInfo(isHit, intersectionPoint, distance, normal);
	}

	Tuple getNormal(Tuple intersectionPoint) const
	{
		// world to local sphere space
		Tuple objectPoint = this->getInverseTransformation() * intersectionPoint;
		Tuple objectNormal = this->getLocalNormal(objectPoint);
		// normal vector back to world
		Tuple worldNormal = this->getTransposedInverseTransformation() * objectNormal;
		worldNormal = Tuple::Vector(worldNormal.X(), worldNormal.Y(), worldNormal.Z());
		return worldNormal.Normalize();
	}

	Matrix<4, 4> getInverseTransformation() const { return invTransformation; }
	Matrix<4, 4> getTransposedInverseTransformation() const { return transposedInvTransformation; }

	MaterialPtr getMaterial() const { return material; }

protected:
	MaterialPtr material;
	Matrix<4, 4> invTransformation;
	Matrix<4, 4> transposedInvTransformation;

	virtual std::pair<bool, float> LocalIntersect(const Ray& ray) const = 0;
	virtual Tuple getLocalNormal(const Tuple& point) const = 0;
};

using ObjectPtr = std::shared_ptr<Object>;