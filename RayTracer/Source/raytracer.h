//---------------------------------------------------------------------
// raytracer.h Header
//---------------------------------------------------------------------
#ifndef RAYTRACER_H
#define RAYTRACER_H

#pragma once

#pragma warning(push, 0)
#include <string.h>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "tuple.h"
#include "scene.h"

class RAYTRACER_EXPORT RayTracer
{
public:
	RayTracer() {};
	unsigned char* execute(Scene scene);
};

#endif