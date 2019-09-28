//---------------------------------------------------------------------
// camera.cpp
//---------------------------------------------------------------------
#include "camera.h"

Camera::Camera(Vector3 lookFromPoint, Vector3 lookAtPoint, Vector3 viewUpVector, float fieldOfViewAngleY, float width, float height)
{
	//1. The origin of each ray, O, is precisely LF.
	this->origin = lookFromPoint;

	//2 Calculating fovx, fovy

	float aspectRatio = (float)width / (float)height;
	this->fovy = glm::radians(fieldOfViewAngleY);
	this->fovx = 2 * atan(tan(this->fovy / 2) * aspectRatio);

	//3. Calculating the new coordinate system - u, v, w, that the camera defines i.e. determine the three orthogonal vectors that are the 
	//		axes of the coordinate system, U, V, and W

	//3.1. W is the viewing direction:
	//            LA-LF
	//     W = -----------
	//         || LA-LF ||

	this->w = lookAtPoint - lookFromPoint;
	this->w.Normalize();

	//3.2. U corresponds to the "X" axis
	//            W x UP
	//     U = -------------
	//         || W x UP ||

	this->u = Cross(w, viewUpVector);
	this->u.Normalize();

	//2.3. V is the "Up" vector
	//          W x U
	//    V = -----------
	//       || W x U ||

	this->v = Cross(w, u);
	this->v.Normalize();

	this->width = width;
	this->height = height;
}

Vector3 Camera::GetDirectionRayForPixel(float x, float y)
{
	//ray direction = w + alpha * u + beta * v
	//
	//			tan(fovx/2) * (y - width/2)
	// alpha = -------------------------------
	//				      width/2
	//
	//			tan(fovy/2) * (height/2 - x)
	// beta = -------------------------------
	//				      height/2


	Vector3 direction = Vector3();
	float alpha = tan(this->fovx / 2)*(y - this->width / 2) / (this->width / 2);
	float beta = tan(this->fovy / 2)*(this->height / 2 - x) / (this->height / 2);
	direction = this->w + this->u * alpha + this->v * beta;
	direction.Normalize();

	/*
	Vector3 direction = Vector3();

	// 1. Raster space
	// add 0.5 to centralize in the middle of the pixel
	float xRasterCentralized = x + 0.5f;
	float yRasterCentralized = y + 0.5f;

	// 2. NDC space (Normalized device coordinates) - range [0:1]
	float xNDC = xRasterCentralized / (this->width);
	float yNDC = yRasterCentralized / (this->height);

	// 3. Screen space (from range [0:1] to range [-1:1]
	float aspectRatio = (float)width / (float)height;
	float xScreen = (2 * xNDC - 1)* tan(this->fovx / 2) *aspectRatio;
	float yScreen = (-2 * yNDC + 1)* tan(this->fovy / 2) *aspectRatio;

	direction = this->w + this->u * xScreen + this->v * yScreen;

	direction.Normalize();
	*/

	return direction;
}

Vector3 Camera::GetOrigin()
{
	return this->origin;
}

Camera::Camera()
{
	this->origin = Vector3();

	this->w = Vector3();
	this->u = Vector3();
	this->u = Vector3();

	this->fovx = 0.5f;
	this->fovy = 0.5f;

	this->width = width;
	this->height = height;
}
