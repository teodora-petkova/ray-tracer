//---------------------------------------------------------------------
// camera.cpp
//---------------------------------------------------------------------
#include "camera.h"

std::tuple<Vector3, Vector3, Vector3> calculateWUV(Vector3 lookAt, Vector3 lookFrom, Vector3 viewUp)
{
	Vector3 w = Vector3();
	Vector3 u = Vector3();
	Vector3 v = Vector3();

	// Calculating the new coordinate system - u, v, w, that the camera defines i.e. determine the three orthogonal vectors that are the 
	//		axes of the coordinate system, U, V, and W

	//3.1. W is the viewing direction:
	//            LA-LF
	//     W = -----------
	//         || LA-LF ||

	w = lookAt - lookFrom;
	w.normalize();

	//3.2. U corresponds to the "X" axis
	//            W x UP
	//     U = -------------
	//         || W x UP ||

	viewUp.normalize();
	u = cross(w, viewUp);
	u.normalize();

	//2.3. V is the "Up" vector
	//          W x U
	//    V = -----------
	//       || W x U ||

	v = cross(u, w);
	v.normalize();

	return std::make_tuple(w, u, v);
}


Camera::Camera(Vector3 lookFromPoint, Vector3 lookAtPoint, Vector3 viewUpVector,
	float fieldOfViewAngleY, int width, int height)
{
	this->width = width;
	this->height = height;

	this->lookFrom = lookFromPoint;
	this->lookAt = lookAtPoint;
	this->viewUp = viewUpVector;

	// The origin of each ray, O, is precisely LF.
	this->origin = lookFromPoint;

	//Calculating fovx, fovy
	float aspectRatio = (float)this->width / (float)this->height;
	this->fovy = glm::radians(fieldOfViewAngleY);
	this->fovx = 2 * atan(tan(this->fovy / 2) * aspectRatio);

	auto t = calculateWUV(this->lookAt, this->lookFrom, this->viewUp);
	this->w = std::get<0>(t);
	this->u = std::get<1>(t);
	this->v = std::get<2>(t);
}

Vector3 Camera::getDirectionRayForPixel(int x, int y)
{
	Vector3 direction = Vector3();

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

	direction = this->u * xScreen + this->v * yScreen + this->w;
	direction.normalize();

	return direction;
}

Vector3 Camera::getOrigin()
{
	return this->origin;
}

void Camera::updateLookAt(int x, int y)
{
	this->lookAt = Vector3(this->lookAt.x + x, this->lookAt.y + y, this->lookAt.z);

	auto t = calculateWUV(this->lookAt, this->lookFrom, this->viewUp);
	this->w = std::get<0>(t);
	this->u = std::get<1>(t);
	this->v = std::get<2>(t);
}

Camera::Camera()
{
	this->origin = Vector3();

	this->w = Vector3();
	this->u = Vector3();
	this->v = Vector3();

	this->fovx = 0.5f;
	this->fovy = 0.5f;

	this->width = width;
	this->height = height;
}
