//---------------------------------------------------------------------
// distantlight.h Header
//---------------------------------------------------------------------
#ifndef DISTANTLIGHT_H
#define DISTANTLIGHT_H

#pragma once

#include "light.h"

typedef Vector3 Color;

class DistantLight : Light
{
public:
	DistantLight(Vector3 position, Color colour, float intensity);
};
#endif