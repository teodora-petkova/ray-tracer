#include "transformations.h"

Matrix<4, 4> Transformations::Translation(float x, float y, float z)
{
	return Matrix<4, 4>({
		{1, 0, 0, x},
		{0, 1, 0, y},
		{0, 0, 1, z},
		{0, 0, 0, 1} });
}

Matrix<4, 4> Transformations::Scaling(float x, float y, float z)
{
	return Matrix<4, 4>({
		{x, 0, 0, 0},
		{0, y, 0, 0},
		{0, 0, z, 0},
		{0, 0, 0, 1} });
}

Matrix<4, 4> Transformations::RotationX(float angleInDegrees)
{
	float angle = degreesToRadians(angleInDegrees);
	return Matrix<4, 4>({
		{	1,	   		 0,			  0, 0},
		{	0,  cos(angle), -sin(angle), 0},
		{	0,	sin(angle),	 cos(angle), 0},
		{	0,			 0,	   		  0, 1} });
}

Matrix<4, 4> Transformations::RotationY(float angleInDegrees)
{
	float angle = degreesToRadians(angleInDegrees);
	return Matrix<4, 4>({
		{	 cos(angle),  0, sin(angle), 0},
		{	          0,  1,          0, 0},
		{	-sin(angle),  0, cos(angle), 0},
		{	          0,  0,  		  0, 1} });
}

Matrix<4, 4> Transformations::RotationZ(float angleInDegrees)
{
	float angle = degreesToRadians(angleInDegrees);
	return Matrix<4, 4>({
		{	cos(angle),	 -sin(angle), 0, 0},
		{	sin(angle),   cos(angle), 0, 0},
		{            0,	           0, 1, 0},
		{	         0,			   0, 0, 1} });
}

Matrix<4, 4> Transformations::Shearing(float a, float b, float c, float d, float e, float f)
{
	return Matrix<4, 4>({
		{1, a, b, 0},
		{c, 1, d, 0},
		{e, f, 1, 0},
		{0, 0, 0, 1} });
}

static std::tuple<Tuple, Tuple, Tuple> calculateWUV(const Tuple& lookFrom, const Tuple& lookAt, const Tuple& viewUp)
{
	// Calculating the new coordinate system - u, v, w, that the camera defines i.e. determine the three orthogonal vectors that are the 
	//		axes of the coordinate system, U, V, and W

	//3.1. W is the viewing direction:
	//            LA-LF
	//     W = -----------
	//         || LA-LF ||
	Tuple w = (lookAt - lookFrom).Normalize();

	//3.2. U corresponds to the "X" axis
	//            W x UP
	//     U = -------------
	//         || W x UP ||
	Tuple u = w.Cross(viewUp.Normalize());//.Normalize();

	//2.3. V is the "Up" vector
	//          W x U
	//    V = -----------
	//       || W x U ||
	Tuple v = u.Cross(w);//.Normalize();

	return std::make_tuple(w, u, v);
}

Matrix<4, 4> Transformations::ViewTransform(Tuple from, Tuple to, Tuple up)
{
	auto t = calculateWUV(from, to, up);
	Tuple forward = std::get<0>(t);
	Tuple left = std::get<1>(t);
	Tuple trueUp = std::get<2>(t);

	Matrix<4, 4> orientation = Matrix<4, 4>({
		{    left.X(),     left.Y(),     left.Z(), 0 },
		{  trueUp.X(),   trueUp.Y(),   trueUp.Z(), 0 },
		{-forward.X(), -forward.Y(), -forward.Z(), 0 },
		{           0,            0,            0, 1 } });

	return orientation * Transformations::Translation(-from.X(), -from.Y(), -from.Z());
}

Matrix<4, 4> Transformations::PerspectiveMatrix(float d, float aspectRatio, float near, float far)
{
	//float cot = std::tanf(M_PI_2 - (fovy / 2.0f));

	float a = (far + near) / (far - near);
	float b = (2 * far * near) / (far - near);

	return Matrix<4, 4>({
		{(d / aspectRatio), 0, 0, 0, 0},
		{0,d, 0, 0, 0},
		{0, 0, a, -1},
		{0, 0, b, 0} });
}