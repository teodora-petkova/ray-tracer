#pragma once

#include "raytracer_exports.h"
#include "tuple.h"
#include "color.h"
#include "pattern.h"
#include "flatcolor.h"
#include "stripepattern.h"
#include "gradient.h"
#include "ringpattern.h"
#include "checkerpattern.h"
#include "radialgradient.h"
#include "blendingpattern.h"

class RAYTRACER_EXPORT Material
{
public:
	Material() :
		pattern(std::make_shared<FlatColor>()),
		ambient(0.1f),
		diffuse(0.9f),
		specular(0.9f),
		shininess(200.f),
		reflective(0.f),
		transparency(0.f),
		refractiveIndex(1.f)
	{}

	Material(PatternPtr pattern,
		float ambient = 0.1f,
		float diffuse = 0.9f,
		float specular = 0.9f,
		float shininess = 200.f,
		float reflective = 0.f,
		float transparency = 0.f,
		float refractiveIndex = 1.f) :
		pattern(pattern),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		shininess(shininess),
		reflective(reflective),
		transparency(transparency),
		refractiveIndex(refractiveIndex)
	{}

	PatternPtr getPattern() const { return pattern; }
	float getAmbient() const { return ambient; }
	float getDiffuse() const { return diffuse; }
	float getShininess() const { return shininess; }
	float getSpecular() const { return specular; }
	float getReflective() const { return reflective; }
	float getTransparency() const { return transparency; }
	float getRefractiveIndex() const { return refractiveIndex; }

private:
	PatternPtr pattern;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
	float reflective;
	float transparency;
	float refractiveIndex;
};