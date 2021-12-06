//---------------------------------------------------------------------
// light.h Header
//---------------------------------------------------------------------
#ifndef LIGHT_H
#define LIGHT_H

#pragma once

#include "raytracer_exports.h"
#include "tuple.h"
#include "color.h"
#include "material.h"

class RAYTRACER_EXPORT Light
{
public:
	Light() :
		position(Tuple::Point(0, 0, 0)),
		color(Color::White()),
		ambient(1),
		diffuse(1),
		specular(1),
		brightness(1)
	{}

	Light(Tuple pos) : Light()
	{
		this->position = pos;
	}

	Light(const Tuple& position,
		const Color& colour,
		float brightness,
		float ambientIntensity,
		float diffuseIntensity,
		float specularIntensity) :
		position(position),
		color(colour),
		brightness(brightness),
		ambient(ambientIntensity),
		diffuse(diffuseIntensity),
		specular(specularIntensity)
	{}

	Color CalculatePhongColor(const Tuple& intersectionPoint,
		const Tuple& unitNormal,
		const Tuple& unitCamera,
		MaterialPtr material,
		bool isInShadow) const;

	Tuple getPosition() const { return position; }
	Color getColor() const { return color; }
	float getBrightness() const { return brightness; }
	float getAmbient() const { return ambient; }
	float getDiffuse() const { return diffuse; }
	float getSpecular() const { return specular; }

private:
	Tuple position;
	Color color;
	float brightness;
	float ambient;
	float diffuse;
	float specular;
};

using LightPtr = std::shared_ptr<Light>;

#endif