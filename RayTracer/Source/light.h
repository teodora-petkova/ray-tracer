//---------------------------------------------------------------------
// light.h Header
//---------------------------------------------------------------------
#ifndef LIGHT_H
#define LIGHT_H

#pragma once

#include "vector3.h"
#include "material.h"

typedef Vector3 Color;

class __declspec(dllexport) Light
{
public:
	Light();
	Light(Vector3 position, Color colour,
		float brightness,
		float ambientIntensity,
		float diffuseIntensity,
		float specularIntensity);

	Color getPhongColor(Vector3 intersection_point,
		Vector3 unit_normal, Vector3 unit_camera,
		Material* material);

	Vector3 getPosition()
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
	Vector3 position;
	Color color;
	float brightness;
	float ambient;
	float diffuse;
	float specular;
};
#endif