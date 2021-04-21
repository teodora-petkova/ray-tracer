//---------------------------------------------------------------------
// material.h Header
//---------------------------------------------------------------------
#ifndef MATERIAL_H
#define MATERIAL_H

#include "raytracer_exports.h"
#include "tuple.h"
#include "color.h"

class RAYTRACER_EXPORT Material
{
public:
	Material();

	Material(Color color, float ambient, float diffuse, float specular,
		float shininess);

	Color getColor()
	{
		return color;
	}

	float getAmbient()
	{
		return ambient;
	}

	float getDiffuse()
	{
		return diffuse;
	}

	float getShininess()
	{
		return shininess;
	}

	float getSpecular()
	{
		return specular;
	}

private:
	Color color;
	float ambient;
	float diffuse;
	float specular;
	float shininess;

};
#endif