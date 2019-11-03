//---------------------------------------------------------------------
// raytracer.h Header
//---------------------------------------------------------------------
#ifndef RAYTRACER_H
#define RAYTRACER_H

#pragma once

#include <string>
#include "vector3.h"

class __declspec(dllexport) RayTracer
{
public:
	RayTracer() {};
	void execute(std::string sceneFileName, std::string outputImageFileName);
};

#endif