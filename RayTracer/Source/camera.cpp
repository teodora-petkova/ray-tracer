//---------------------------------------------------------------------
// camera.cpp
//---------------------------------------------------------------------
#include "camera.h"

std::tuple<Tuple, Tuple, Tuple> calculateWUV(Tuple& lookAt, Tuple& lookFrom, Tuple& viewUp)
{
	Tuple w = Tuple();
	Tuple u = Tuple();
	Tuple v = Tuple();

	// Calculating the new coordinate system - u, v, w, that the camera defines i.e. determine the three orthogonal vectors that are the 
	//		axes of the coordinate system, U, V, and W

	//3.1. W is the viewing direction:
	//            LA-LF
	//     W = -----------
	//         || LA-LF ||

	w = (lookAt - lookFrom).normalize();

	//3.2. U corresponds to the "X" axis
	//            W x UP
	//     U = -------------
	//         || W x UP ||

	viewUp = viewUp.normalize();

	u = Tuple::cross(w, viewUp).normalize();

	//2.3. V is the "Up" vector
	//          W x U
	//    V = -----------
	//       || W x U ||

	v = Tuple::cross(u, w).normalize();

	return std::make_tuple(w, u, v);
}

#define degreesToRadians(x) x*(3.141592f/180.0f)

Camera::Camera(Tuple lookFromPoint, Tuple lookAtPoint, Tuple viewUpVector,
	float fieldOfViewAngleY, int width, int height)
{
	this->width = width;
	this->height = height;

	this->lookFrom = lookFromPoint;
	this->lookAt = lookAtPoint;
	this->viewUp = viewUpVector;

	//Calculating fovx, fovy
	float aspectRatio = (float)this->width / (float)this->height;
	this->fovy = degreesToRadians(fieldOfViewAngleY);
	this->fovx = 2 * atan(tan(this->fovy / 2) * aspectRatio);

	auto t = calculateWUV(this->lookAt, this->lookFrom, this->viewUp);
	this->w = std::get<0>(t);
	this->u = std::get<1>(t);
	this->v = std::get<2>(t);
}

Tuple Camera::getDirectionRayForPixel(int x, int y)
{
	Tuple direction = Tuple();

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

	direction = (this->u * xScreen + this->v * yScreen + this->w).normalize();

	return direction;
}

Tuple Camera::getOrigin()
{
	return this->lookFrom;
}

void Camera::updateLookAt(int x, int y)
{
	Tuple updatedLookAt = Tuple::Vector(this->lookAt.x + x, this->lookAt.y + y, this->lookAt.z);

	auto t = calculateWUV(updatedLookAt, this->lookFrom, this->viewUp);
	this->w = std::get<0>(t);
	this->u = std::get<1>(t);
	this->v = std::get<2>(t);
}

Camera::Camera()
{
	this->w = Tuple();
	this->u = Tuple();
	this->v = Tuple();

	this->fovx = 0.5f;
	this->fovy = 0.5f;

	this->width = width;
	this->height = height;
}
