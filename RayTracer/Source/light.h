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
	Light();
	Light(Tuple position, Color colour,
		float brightness,
		float ambientIntensity,
		float diffuseIntensity,
		float specularIntensity);

	Color getPhongColor(Tuple intersection_point,
		Tuple unit_normal, Tuple unit_camera,
		MaterialPtr material);

	Tuple getPosition()
	{
		return position;
	}

	Color getColor()
	{
		return color;
	}

	float getBrightness()
	{
		return brightness;
	}

	float getAmbient()
	{
		return ambient;
	}

	float getDiffuse()
	{
		return diffuse;
	}

	float getSpecular()
	{
		return specular;
	}

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