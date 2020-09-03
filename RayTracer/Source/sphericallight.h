//---------------------------------------------------------------------
// sphericallight.h Header
//---------------------------------------------------------------------
#ifndef SPHERICALLIGHT_H
#define SPHERICALLIGHT_H

#pragma once

#include "light.h"

typedef Vector3 Color;

class SphericalLight : Light
{
public:
	SphericalLight(Vector3 position, Color colour, float intensity);
};
#endif