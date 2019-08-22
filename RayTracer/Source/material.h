//---------------------------------------------------------------------
// material.h Header
//---------------------------------------------------------------------
#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector3.h"

typedef Vector3 Color;

class Material
{
public:
	Material();

	void SetColor(Color &color)
	{
		materialColor = color;
	}

	void SetColor(float r, float g, float b)
	{
		materialColor.x = r;
		materialColor.y = g;
		materialColor.z = b;
	}

	Color GetColor()
	{
		return materialColor;
	}

private:
	Color materialColor;
};
#endif