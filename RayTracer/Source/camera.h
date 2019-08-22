//---------------------------------------------------------------------
// camera.h Header
//---------------------------------------------------------------------
#ifndef CAMERA_H
#define CAMERA_H

#pragma once

#include "vector3.h"
#include <glm/glm.hpp>

typedef Vector3 Color;

class Camera
{
public:
	Camera();
	Camera(Vector3 lookFromPoint, Vector3 lookAtPoint, Vector3 viewUpVector, float fieldOfViewAngleY, float width, float height);
	Vector3 GetDirectionRayForPixel(float x, float y);
	Vector3 Camera::GetOrigin();
private:
	Vector3 origin;
	Vector3 w;
	Vector3 u;
	Vector3 v;
	float fovx;
	float fovy;
	float width;
	float height;
};
#endif