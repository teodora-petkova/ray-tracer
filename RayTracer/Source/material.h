//---------------------------------------------------------------------
// material.h Header
//---------------------------------------------------------------------
#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector3.h"

typedef Vector3 Color;

class __declspec(dllexport) Material
{
public:
	Material();

	Material(Color color, float ambient, float diffuse, float specular,
		int shininess);

	Color getColor()
	{
		return materialColor;
	}

	float getAmbient()
	{
		return ambient;
	}

	float getDiffuse()
	{
		return diffuse;
	}

	int getShininess()
	{
		return shininess;
	}

	float getSpecular()
	{
		return specular;
	}

private:
	Color materialColor;
	float ambient;
	float diffuse;
	float specular;
	int shininess;

};
#endif