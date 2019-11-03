//---------------------------------------------------------------------
// light.h Header
//---------------------------------------------------------------------
#ifndef LIGHT_H
#define LIGHT_H

#pragma once

#include "vector3.h"

typedef Vector3 Color;

class Light
{
public:
	Light();
	Light(Vector3 position, Color colour, float intensity);
	Vector3 Position;
	Color Colour;
	Vector3 Intensity;
};
#endif