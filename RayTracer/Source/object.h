//------------------------------------------------------------
// object.h Header
//------------------------------------------------------------
#ifndef OBJECT_H
#define OBJECT_H

#include "raytracer_exports.h"
#include "tuple.h"
#include "ray.h"
#include "material.h"
#include "intersectioninfo.h"

class RAYTRACER_EXPORT Object
{
public:
	Object() :
		material(nullptr),
		transformation(Matrix<4, 4>::IdentityMatrix())
	{}

	Object(MaterialPtr material, Matrix<4, 4> matrix) :
		material(material),
		transformation(matrix.Inverse()),
		transposedTransformation(this->transformation.Transpose())
	{}

	virtual IntersectionInfo Intersect(const Ray& ray) const = 0;

	Matrix<4, 4> getTransformation() const { return transformation; }
	Matrix<4, 4> getTransposedTransformation() const { return transposedTransformation; }

	MaterialPtr getMaterial() const { return material; }

protected:
	MaterialPtr material;
	Matrix<4, 4> transformation;
	Matrix<4, 4> transposedTransformation;
};

using ObjectPtr = std::shared_ptr<Object>;

#endif