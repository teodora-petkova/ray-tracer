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
		shininess(200),
		reflective(0)
	{}

	Material(PatternPtr pattern,
		float ambient, float diffuse,
		float specular, float shininess,
		float reflective) :
		pattern(pattern),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		shininess(shininess),
		reflective(reflective)
	{}

	PatternPtr getPattern() const { return pattern; }
	float getAmbient() const { return ambient; }
	float getDiffuse() const { return diffuse; }
	float getShininess() const { return shininess; }
	float getSpecular() const { return specular; }
	float getReflective() const { return reflective; }

private:
	PatternPtr pattern;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
	float reflective;
};

using MaterialPtr = std::shared_ptr<Material>;