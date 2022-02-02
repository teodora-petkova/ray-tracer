#pragma once

#pragma warning(push, 0)
#include <memory>
#include <vector>
#include <algorithm>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "tuple.h"
#include "types.h"
#include "ray.h"
#include "material.h"
#include "boundingbox.h"
#include "intersectioninfo.h"

class RAYTRACER_EXPORT Object : public std::enable_shared_from_this<Object>
{
public:
	Object() :
		material(std::make_shared<Material>()),
		transformation(Matrix<4, 4>::IdentityMatrix()),
		invTransformation(Matrix<4, 4>::IdentityMatrix()),
		transposedInvTransformation(Matrix<4, 4>::IdentityMatrix()),
		bounds(Tuple::Point(INFINITY, INFINITY, INFINITY),
			Tuple::Point(-INFINITY, -INFINITY, -INFINITY)),
		boundsInParentSpace(bounds)
	{}

	Object(MaterialPtr material, Matrix<4, 4> matrix) :
		material(material),
		bounds(Tuple::Point(INFINITY, INFINITY, INFINITY),
			Tuple::Point(-INFINITY, -INFINITY, -INFINITY)),
		boundsInParentSpace(bounds)
	{
		Matrix<4, 4> inverseTransformation = matrix.Inverse();
		this->transformation = matrix;
		this->invTransformation = inverseTransformation;
		this->transposedInvTransformation = inverseTransformation.Transpose();
	}

	IntersectionInfo Intersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const;

	Tuple getNormal(Tuple intersectionPoint,
		const IntersectionParams& intersection = IntersectionParams()) const;

	Tuple TransformFromWorldToObjectSpace(Tuple point) const;
	Tuple TransformFromObjectToWorldSpace(Tuple vector) const;

	Matrix<4, 4> getInverseTransformation() const { return invTransformation; }
	Matrix<4, 4> getTransposedInverseTransformation() const { return transposedInvTransformation; }

	MaterialPtr getMaterial() const { return material; }
	void setParent(ObjectPtr parentGroup) { this->parent = parentGroup; }
	ObjectConstPtr getParent() const { return this->parent; }

	void setMaterial(MaterialPtr material)
	{
		this->material = material;
	}

	BoundingBox getBounds() const { return bounds; }
	BoundingBox getBoundsInParentSpace() const { return boundsInParentSpace; }

protected:
	MaterialPtr material;
	Matrix<4, 4> transformation;
	Matrix<4, 4> invTransformation;
	Matrix<4, 4> transposedInvTransformation;
	ObjectPtr parent = nullptr;
	BoundingBox bounds;
	BoundingBox boundsInParentSpace;

	virtual IntersectionParams LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const = 0;
	virtual Tuple getLocalNormal(const Tuple& point, const IntersectionParams& /*intersection*/) const = 0;
};