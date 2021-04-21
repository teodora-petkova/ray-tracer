//---------------------------------------------------------------------
// camera.h Header
//---------------------------------------------------------------------
#ifndef CAMERA_H
#define CAMERA_H

#pragma once

#pragma warning(push, 0)
#include <tuple>
#pragma warning(pop)

#include "raytracer_exports.h"
#include "tuple.h"

class RAYTRACER_EXPORT Camera
{
public:
	Camera();
	Camera(Tuple lookFromPoint, Tuple lookAtPoint, Tuple viewUpVector, float fieldOfViewAngleY, int width, int height);
	Tuple getDirectionRayForPixel(int x, int y);
	Tuple getOrigin();
	void updateLookAt(int x, int y);
private:
	Tuple origin;
	Tuple lookAt;
	Tuple lookFrom;
	Tuple viewUp;
	Tuple w;
	Tuple u;
	Tuple v;
	float fovx;
	float fovy;
	int width;
	int height;
};
#endif