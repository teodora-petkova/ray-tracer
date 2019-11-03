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

	void setColor(Color &color)
	{
		materialColor = color;
	}

	void setColor(float r, float g, float b)
	{
		materialColor.x = r;
		materialColor.y = g;
		materialColor.z = b;
	}

	Color getColor()
	{
		return materialColor;
	}

private:
	Color materialColor;
};
#endif