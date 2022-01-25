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
#include "intersectioninfo.h"

class RAYTRACER_EXPORT Object : public std::enable_shared_from_this<Object>
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
		this->SetTransformation(matrix);
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

	void SetMaterial(MaterialPtr material)
	{
		this->material = material;
	}

	void SetTransformation(Matrix<4, 4> matrix)
	{
		Matrix<4, 4> inverseTransformation = matrix.Inverse();
		this->invTransformation = inverseTransformation;
		this->transposedInvTransformation = inverseTransformation.Transpose();
	}

protected:
	MaterialPtr material;
	Matrix<4, 4> invTransformation;
	Matrix<4, 4> transposedInvTransformation;
	ObjectPtr parent = nullptr;

	virtual IntersectionParams LocalIntersect(const Ray& ray,
		std::vector<std::pair<float, ObjectConstPtr>>& intersectionDistances) const = 0;
	virtual Tuple getLocalNormal(const Tuple& point, const IntersectionParams& /*intersection*/) const = 0;
};