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
		color(Color(0.2f, 0.2f, 0.2f)),
		brightness(0.9f),
		ambient(0.9f),
		diffuse(0.9f),
		specular(0.9f)
	{}

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

	Color CalculatePhongColor(const Tuple& intersection_point,
		const Tuple& unit_normal,
		const Tuple& unit_camera,
		MaterialPtr material) const;

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