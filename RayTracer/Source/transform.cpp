// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

//Takes as input current eye position, and current up vector.
//up is always normalized to a length of 1.
//eye length indicates distance from viewer to origin

// Helper rotation function.  Please implement this.  
// Please ignore "degrees" below; it's actually in radians, 
// since "rotate" is always called with an angle in radians
// Axis is already normalized when passed in by left or up.
mat3 Transform::rotate(const float radians, const vec3& axis)
{
	vec3 unitVector = glm::normalize(axis);

	mat3 identityMatrix = glm::mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);

	mat3 matrixAAtranspose = glm::mat3(
		unitVector.x*unitVector.x, unitVector.x * unitVector.y, unitVector.x * unitVector.z,
		unitVector.x*unitVector.y, unitVector.y * unitVector.y, unitVector.y * unitVector.z,
		unitVector.x*unitVector.z, unitVector.y * unitVector.z, unitVector.z * unitVector.z);

	mat3 dualMatrix = glm::mat3(
		0, -unitVector.z, unitVector.y,
		unitVector.z, 0, -unitVector.x,
		-unitVector.y, unitVector.x, 0);

	mat3 matrixRotated = (float)cos(radians) * identityMatrix + (float)(1 - cos(radians)) * matrixAAtranspose + (float)sin(radians) * dualMatrix;
	return matrixRotated;
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
	// Convert to radians (mult by pi/180) before calling rotate
	// Rotate up and eye by degrees about the up direction
	mat3 m = rotate(glm::radians(degrees), up);
	eye = eye * m;
	up = up * m;

}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
	// Convert to radians (mult by pi/180) before calling rotate
	// Rotate eye and up by degrees about the third direction
	// Third direction is obtained by cross producting eye and up.  
	vec3 v = glm::cross(eye, up);
	mat3 m = rotate(glm::radians(degrees), v);
	eye = eye * m;
	up = up * m;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
	vec3 w = glm::normalize(eye);
	vec3 u = glm::normalize(glm::cross(up, w));
	vec3 v = glm::cross(w, u);
	/*
	mat4 mRotation = mat4(
		u.x, u.y, u.z, 0,
		v.x, v.y, v.z, 0,
		w.x, w.y, w.z, 0,
		0, 0, 0, 1);

	mat4 mTranslation = mat4(
		1, 0, 0, -eye.x,
		0, 1, 0, -eye.y,
		0, 0, 1, -eye.z,
		0, 0, 0, 1);
	mat4 mResult = mRotation *  mTranslation;
	*/
	mat4 mResult = mat4(
		u.x, u.y, u.z, -u.x*eye.x - u.y*eye.y - u.z*eye.z,
		v.x, v.y, v.z, -v.x*eye.x - v.y*eye.y - v.z*eye.z,
		w.x, w.y, w.z, -w.x*eye.x - w.y*eye.y - w.z*eye.z,
		0, 0, 0, 1);

	return mResult;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	mat4 ret;

	float F = cos(glm::radians(fovy / 2)) / sin(glm::radians(fovy / 2));
	ret = mat4(F / aspect, 0.0, 0.0, 0.0,
		0.0, F, 0.0, 0.0,
		0.0, 0.0, -(zNear + zFar) / (zFar - zNear), -(2 * zNear*zFar) / (zFar - zNear),
		0.0, 0.0, -1.0, 0.0);
	return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
	mat4 ret;

	ret = mat4(sx, 0.0, 0.0, 0.0,
		0.0, sy, 0.0, 0.0,
		0.0, 0.0, sz, 0.0,
		0.0, 0.0, 0.0, 1.0);
	return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
	mat4 ret;

	ret = mat4(1.0, 0.0, 0.0, tx,
		0.0, 1.0, 0.0, ty,
		0.0, 0.0, 1.0, tz,
		0.0, 0.0, 0.0, 1.0);
	return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
	vec3 x = glm::cross(up, zvec);
	vec3 y = glm::cross(zvec, x);
	vec3 ret = glm::normalize(y);
	return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
