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
#include "ray.h"
#include "matrix.h"
#include "transformations.h"

class RAYTRACER_EXPORT Camera
{
public:
	Camera();
	Camera(const Tuple& lookFromPoint, const Tuple& lookAtPoint, const Tuple& viewUpVector,
		float fieldOfViewAngleY, int width, int height);

	Ray CalculateRayForPixel(int x, int y) const;
	Ray CalculateRayForPixel2(int x, int y) const;

	void setTransform(const Matrix<4, 4>& matrix);

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	float getFOV() const { return fovy; }
	Tuple getOrigin() const { return origin; }
	std::pair<float, float> getPixelSize() const { return pixelSize; }

private:
	float fovx;
	float fovy;
	int width;
	int height;

	std::pair<float, float> pixelSize;
	float halfWidth;
	float halfHeight;

	Matrix<4, 4> transform;
	Tuple origin;
};
#endif