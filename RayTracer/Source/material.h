//---------------------------------------------------------------------
// material.h Header
//---------------------------------------------------------------------
#ifndef MATERIAL_H
#define MATERIAL_H

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

class RAYTRACER_EXPORT Material
{
public:
	Material() :
		pattern(std::make_shared<FlatColor>()),
		ambient(0.1f),
		diffuse(0.9f),
		specular(0.9f),
		shininess(200)
	{}

	Material(PatternPtr pattern,
		float ambient, float diffuse,
		float specular, float shininess) :
		pattern(pattern),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		shininess(shininess)
	{}

	PatternPtr getPattern() const { return pattern; }
	float getAmbient() const { return ambient; }
	float getDiffuse() const { return diffuse; }
	float getShininess() const { return shininess; }
	float getSpecular() const { return specular; }

private:
	PatternPtr pattern;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

using MaterialPtr = std::shared_ptr<Material>;

#endif