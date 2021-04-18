//---------------------------------------------------------------------
// raytracer.h Header
//---------------------------------------------------------------------
#ifndef RAYTRACER_H
#define RAYTRACER_H

#pragma once

#include <string>
#include "Tuple.h"
#include "scene.h"

class __declspec(dllexport) RayTracer
{
public:
	RayTracer() {};
	unsigned char* execute(Scene scene);
};

#endif