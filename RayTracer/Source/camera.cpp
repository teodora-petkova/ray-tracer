//---------------------------------------------------------------------
// camera.cpp
//---------------------------------------------------------------------
#include "camera.h"

Camera::Camera()
{
	this->transform = Transformations::ViewTransform(
		Tuple::Point(0, 0, 0),
		Tuple::Vector(0, 0, -1),
		Tuple::Point(0, 1, 0)
	);

	this->fovx = 90;
	this->fovy = 90;

	this->width = 100;
	this->height = 100;

	this->halfHeight = this->height / 2.0f;
	this->halfWidth = this->width / 2.0f;
}

Camera::Camera(int width, int height)
	:Camera()
{
	this->width = width;
	this->height = height;

	this->halfHeight = this->height / 2.0f;
	this->halfWidth = this->width / 2.0f;
}

Camera::Camera(const Tuple& lookFromPoint, const Tuple& lookAtPoint, const Tuple& viewUpVector,
	float fieldOfViewAngleY, int width, int height)
{
	this->width = width;
	this->height = height;

	//Calculating fovx, fovy
	float aspectRatio = (float)this->width / (float)this->height;
	this->fovy = degreesToRadians(fieldOfViewAngleY);
	this->fovx = 2 * atan(tan(this->fovy / 2) * aspectRatio);


	//The canvas is one unit away (z=-1)
	float halfView = tan(fovy / 2.f);

	// the field of view is on the larger part - width or height
	// aspectRatio = width / height
	// width = aspectRatio * height
	// height = width / aspectRatio
	if (aspectRatio >= 1) // width >= height
	{
		this->halfWidth = halfView;
		this->halfHeight = halfView / aspectRatio;
	}
	else // height > width
	{
		this->halfWidth = halfView * aspectRatio;
		this->halfHeight = halfView;
	}
	this->pixelSize = std::make_pair(
		this->halfWidth * 2.f / this->width,
		this->halfHeight * 2.f / this->height);


	this->transform = Transformations::ViewTransform(
		lookFromPoint,
		lookAtPoint,
		viewUpVector).Inverse();

	this->origin = this->transform * Tuple::Point(0, 0, 0);
}

Ray Camera::CalculateRayForPixel(int x, int y) const {
	// summary:
	// 1.) (x, y) - the point/pixel coordinates on the screen
	// 2.) (xWorld, yWorld, -1) - the point in the world (we want to look into z = -1 direction by default!)
	// 3.) (xCamera, yCamera, zCamera) - the same point in the camera coordinate system

	// 1. Raster space - add 0.5 to centralize in the middle of the pixel
	// (x, y) are the upper left corner of the pixel, so we centralize in the middle of the pixel
	// we consider the pixel to be 1 unit
	float xRasterCentralized = x + 0.5f;
	float yRasterCentralized = y + 0.5f;

	//the offset from the edge of the canvas to the pixel's center
	float xOffset = xRasterCentralized * this->pixelSize.first;
	float yOffset = yRasterCentralized * this->pixelSize.second;

	//2. World space - the untransformed coordinates of the pixel in world space.
	// (remember that the camera looks toward -z, so +x is to the *left*.)
	float xWorld = this->halfWidth - xOffset;
	float yWorld = this->halfHeight - yOffset;

	//3. Camera space with the viewing transformation
	// transform the canvas point and the origin using the camera matrix
	// and then compute the ray's direction vector
	// (the camera is looking at the center (0, 0, 0) and the canvas is at z=-1)
	Tuple pixel = this->transform * Tuple::Point(xWorld, yWorld, -1);
	Tuple direction = (pixel - this->origin).Normalize();

	return Ray(this->origin, direction);
}

Ray Camera::CalculateRayForPixel2(int x, int y) const
{
	// 1. Raster space
	// add 0.5 to centralize in the middle of the pixel
	float xRasterCentralized = x + 0.5f;
	float yRasterCentralized = y + 0.5f;

	// 2. NDC space (Normalized device coordinates) - range [0:1]
	float xNDC = xRasterCentralized / this->width;
	float yNDC = yRasterCentralized / this->height;

	// 3. Screen space (from range [0:1] to range [-1:1]
	float xScreen = (2 * xNDC - 1) * tan(this->fovx / 2);
	float yScreen = (-2 * yNDC + 1) * tan(this->fovy / 2);

	Tuple pixel = this->transform * Tuple::Point(xScreen, yScreen, -1);
	Tuple direction = (pixel - this->origin).Normalize();

	return Ray(this->origin, direction);
}

void Camera::setTransform(const Matrix<4, 4>& matrix)
{
	this->transform = (this->transform * matrix).Inverse();

	this->origin = this->transform * Tuple::Point(0, 0, 0);
}